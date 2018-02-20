#pragma once
#include "windows.h"
#include <string>

namespace loader {

    bool LaunchDebugger();
    const std::wstring s2ws(const std::string& str);
    const std::string ws2s(const std::wstring& wstr);

    const std::wstring GetSystemPath();
    const std::string GetSystemPath(const std::string& append);
    const std::wstring GetSystemPath(const std::wstring& append);
    HMODULE LoadSystemD3D9();

}

