#include "Installer.h"
#include "../windows.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <ShlObj.h>
#include <vector>
#include <miniz.h>
#include "../globals.h"
#include "../log.h"
#include "../utils/encoding.h"
#include "../utils/file.h"

using namespace std;
using namespace std::experimental::filesystem::v1;
using namespace loader::addons;
using namespace loader::utils;

namespace loader {
    namespace updaters {

        Installer::Installer(const VersionInfo version) :
            version(version),
            targetSubfolder(BIN_FOLDER),
            targetFileName("d3d9.dll") { }

        Installer::Installer(const VersionInfo version, const Addon* addon) : 
            version(version),
            targetSubfolder(ADDONS_FOLDER),
            targetFileName(addon->GetFileName()) { }


        void Installer::StartInstall() {
            if (this->busy) {
                return;
            }

            this->busy = true;

            this->downloader = unique_ptr<Downloader>(new Downloader(this->version.downloadUrl));
            this->downloader->SetProgressUpdateCallback([this](const Downloader* const downloader, size_t progress, size_t total) {
                this->DownloaderProgressUpdate(downloader, progress, total);
            });
            this->downloader->SetDownloadCompleteCallback([this](const Downloader* const downloader, const vector<char>& data, const string& errorMessage) {
                this->DownloaderComplete(downloader, data, errorMessage);
            });
            this->downloader->StartDownloadAsync();
        }

        const string Installer::GetDetailedProgress() {
            lock_guard<mutex> guard(this->detailedProgressMutex);
            return this->detailedProgress;
        }

        void Installer::SetDetailedProgress(const string& progress) {
            lock_guard<mutex> guard(this->detailedProgressMutex);
            this->detailedProgress = progress;
        }


        void Installer::DownloaderProgressUpdate(const Downloader* const downloader, size_t progress, size_t total) {
            if (total > 0) {
                this->SetDetailedProgress("Downloading (" + to_string(progress) + " / " + to_string(total) + ")");
            }
            else if (progress > 0) {
                this->SetDetailedProgress("Downloading (" + to_string(progress) + ")");
            }
            else if (downloader->IsBusy()) {
                this->SetDetailedProgress("Connecting");
            }
            else {
                this->SetDetailedProgress("");
            }
        }

        void Installer::DownloaderComplete(const Downloader* const downloader, const vector<char>& data, const string& errorMessage) {
            if (!errorMessage.empty()) {
                this->SetDetailedProgress("Error while downloading: " + errorMessage);
                GetLog()->error("Error while downloading " + downloader->GetUrl() + ": " + errorMessage);
                this->busy = false;
            }
            else if (downloader->HasCompleted()) {
                this->SetDetailedProgress("Finished downloading");
                GetLog()->info("Finished downloading " + downloader->GetUrl());
                this->Extract(data);
            }
        }

        void Installer::Extract(const vector<char>& data) {
            path folder = GetGuildWars2Folder(this->targetSubfolder);
            this->extractTask = async(launch::async, [=]() {

                bool success = true;
                vector<path> extractedFiles;

                // Determine first if data is a Windows PE file or a ZIP file
                if (data[0] == 0x4D && data[1] == 0x5A) {
                    // MZ, Windows PE
                    this->SetDetailedProgress("Copying new file");
                    const path target = folder / this->targetFileName;
                    try {
                        this->WriteFile(data, target);
                        extractedFiles.push_back(target);
                        GetLog()->info("Copied new file " + target.string());
                    }
                    catch (const exception& e) {
                        this->SetDetailedProgress(string("Error while copying: ") + e.what());
                        GetLog()->error("Error while copying new file " + target.string() + ": " + e.what());
                        success = false;
                    }
                }
                else if (data[0] == 0x50 && data[1] == 0x4B) {
                    // PK, ZIP archive
                    mz_zip_archive archive;
                    memset(&archive, 0, sizeof(archive));
                    if (!mz_zip_reader_init_mem(&archive, data.data(), data.size(), 0)) {
                        this->SetDetailedProgress("Error while extracting: Could not open the downloaded archive");
                        GetLog()->error(this->GetDetailedProgress());
                        success = false;
                        return;
                    }

                    mz_zip_archive_file_stat fileStat;
                    mz_uint archiveNumFiles = mz_zip_reader_get_num_files(&archive);
                    for (mz_uint i = 0; i < archiveNumFiles; ++i) {
                        this->SetDetailedProgress("Extracting files (" + to_string(i) + " / " + to_string(archiveNumFiles) + ")");

                        if (!mz_zip_reader_file_stat(&archive, i, &fileStat)) {
                            this->SetDetailedProgress("Error while extracting: Could not read at index " + to_string(i));
                            GetLog()->error(this->detailedProgress);
                            success = false;
                            break;
                        }
                        string target = fileStat.m_filename;
                        if (target[target.length() - 1] == '/') {
                            // According to miniz documentation, this is just a subfolder
                            continue;
                        }
                        size_t fileSize;
                        char* fileData = static_cast<char*>(mz_zip_reader_extract_to_heap(&archive, i, &fileSize, 0));
                        if (fileData == NULL) {
                            this->SetDetailedProgress("Error while extracting: Could not extract at index " + to_string(i));
                            GetLog()->error(this->detailedProgress);
                            success = false;
                            break;
                        }

                        try {
                            path target2 = folder / target;
                            this->WriteFile(vector<char>(fileData, fileData + fileSize), target2);
                            // Set the last modified time to the original modified time in archive
                            HANDLE hFile = CreateFile(target2.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                            if (hFile != NULL) {
                                FILETIME fileTime = TimetToFileTime(fileStat.m_time);
                                SetFileTime(hFile, NULL, NULL, &fileTime);
                                CloseHandle(hFile);
                            }
                            extractedFiles.push_back(target2);
                            GetLog()->info("Copied new file " + target2.string());
                        }
                        catch (const exception& e) {
                            this->SetDetailedProgress(string("Error while extracting ") + target + ": " + e.what());
                            GetLog()->error("Error while extracting new file " + target + ": " + e.what());
                            success = false;
                            break;
                        }

                        mz_free(fileData);
                    }

                    mz_zip_reader_end(&archive);
                }

                if (success) {
                    // Delete all back-up files
                    for (auto fileName : extractedFiles) {
                        fileName += ".bak";
                        if (FileExists(fileName.string())) {
                            if (!DeleteFile(fileName.c_str())) {
                                GetLog()->error("Failed to delete back-up file " + fileName.string());
                            }
                        }
                    }

                    this->SetDetailedProgress("Completed successfully");
                }
                else {
                    // Revert all the copied files to the previous version
                    for (auto fileName : extractedFiles) {
                        if (FileExists(fileName.string())) {
                            if (!DeleteFile(fileName.c_str())) {
                                GetLog()->error("Failed to delete file " + fileName.string() + "; manual restore from back-up file required");
                                continue;
                            }
                        }
                        path moveFrom = fileName;
                        moveFrom += ".bak";
                        if (!MoveFile(moveFrom.c_str(), fileName.c_str())) {
                            GetLog()->error("Failed to rename back-up file to its original " + fileName.string());
                        }
                    }
                }

                this->busy = false;
                this->completed = true;
                this->extractTask = {};

            });
        }

        void Installer::WriteFile(const vector<char>& fileData, const path& fileName) {
            // Prevent d3d9 overwrites if it's not supposed to overwrite
            if (fileName.filename() == "d3d9.dll" && this->targetFileName != "d3d9.dll") {
                GetLog()->warn("Prevented overwriting d3d9.dll, please don't include d3d9.dll in addon archive files if the addon depends on that");
                return;
            }

            path folder(fileName);
            folder.remove_filename();
            if (!FolderExists(folder.string()) && SHCreateDirectory(NULL, folder.c_str()) != ERROR_SUCCESS) {
                throw exception(("Failed to create subfolder " + folder.string()).c_str());
            }

            path moveTo = fileName;
            moveTo += ".bak";
            if (!MoveFile(fileName.c_str(), moveTo.c_str())) {
                GetLog()->error("Failed to rename original " + fileName.string() + " to its backed up file name");
            }

            ofstream ofs(fileName, ios::binary);
            ofs.write(fileData.data(), fileData.size());
            ofs.close();
        }

    }
}