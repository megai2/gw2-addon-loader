#pragma once

// Standard
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOWINSTYLES
#define NOMENUS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NODRAWTEXT
#define NOKERNEL
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#include <windows.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <DbgHelp.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <strsafe.h>
#include <WinInet.h>
#include <WinSock2.h>

// Third-party
#include <IconsMaterialDesign.h>
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <json.hpp>
#include <MinHook.h>
#include <miniz.h>
#include <SimpleIni.h>
#include <spdlog/sinks/basic_file_sink.h>

// Project
#include <d3d9-defines.h>
#include <gw2addon-legacy.h>
#include <gw2addon-native.h>
