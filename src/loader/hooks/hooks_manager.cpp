#include "hooks_manager.h"
#include "LoaderDirect3D9.h"
#include "../log.h"
#include "../utils/file.h"

using namespace std;
using namespace std::filesystem;
using namespace loader::utils;

namespace loader::hooks {

    UINT SDKVersion;
    HMODULE SystemD3D9Module = nullptr;
    D3D9Exports SystemD3D9Funcs;


    void InitializeHooks() {
        HOOKS_LOG()->info("Initializing hooks");

        // It's unadvised to call LoadLibrary in DllMain, because it can cause a deadlock.
        // But eh whatever, we are just a proxy anyway, no one should call us except Guild Wars 2.
        path systemD3D9 = GetSystemFolder("d3d9");
        SystemD3D9Module = LoadLibrary(systemD3D9.c_str());
        if (!SystemD3D9Module) {
            HOOKS_LOG()->error("Failed to load the system d3d9.dll: {0}", LastErrorToString());
            return;
        }

        SystemD3D9Funcs.Direct3DShaderValidatorCreate9 = reinterpret_cast<Direct3DShaderValidatorCreate9_t*>(GetProcAddress(SystemD3D9Module, "Direct3DShaderValidatorCreate9"));
        SystemD3D9Funcs.PSGPError = reinterpret_cast<PSGPError_t*>(GetProcAddress(SystemD3D9Module, "PSGPError"));
        SystemD3D9Funcs.PSGPSampleTexture = reinterpret_cast<PSGPSampleTexture_t*>(GetProcAddress(SystemD3D9Module, "PSGPSampleTexture"));
        SystemD3D9Funcs.D3DPERF_BeginEvent = reinterpret_cast<D3DPERF_BeginEvent_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_BeginEvent"));
        SystemD3D9Funcs.D3DPERF_EndEvent = reinterpret_cast<D3DPERF_EndEvent_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_EndEvent"));
        SystemD3D9Funcs.D3DPERF_GetStatus = reinterpret_cast<D3DPERF_GetStatus_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_GetStatus"));
        SystemD3D9Funcs.D3DPERF_QueryRepeatFrame = reinterpret_cast<D3DPERF_QueryRepeatFrame_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_QueryRepeatFrame"));
        SystemD3D9Funcs.D3DPERF_SetMarker = reinterpret_cast<D3DPERF_SetMarker_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_SetMarker"));
        SystemD3D9Funcs.D3DPERF_SetOptions = reinterpret_cast<D3DPERF_SetOptions_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_SetOptions"));
        SystemD3D9Funcs.D3DPERF_SetRegion = reinterpret_cast<D3DPERF_SetRegion_t*>(GetProcAddress(SystemD3D9Module, "D3DPERF_SetRegion"));
        SystemD3D9Funcs.DebugSetLevel = reinterpret_cast<DebugSetLevel_t*>(GetProcAddress(SystemD3D9Module, "DebugSetLevel"));
        SystemD3D9Funcs.DebugSetMute = reinterpret_cast<DebugSetMute_t*>(GetProcAddress(SystemD3D9Module, "DebugSetMute"));
        SystemD3D9Funcs.Direct3D9EnableMaximizedWindowedModeShim = reinterpret_cast<Direct3D9EnableMaximizedWindowedModeShim_t*>(GetProcAddress(SystemD3D9Module, "Direct3D9EnableMaximizedWindowedModeShim"));
        SystemD3D9Funcs.Direct3DCreate9 = reinterpret_cast<Direct3DCreate9_t*>(GetProcAddress(SystemD3D9Module, "Direct3DCreate9"));
        SystemD3D9Funcs.Direct3DCreate9Ex = reinterpret_cast<Direct3DCreate9Ex_t*>(GetProcAddress(SystemD3D9Module, "Direct3DCreate9Ex"));
    }

    void UninitializeHooks() {
        HOOKS_LOG()->info("Uninitializing hooks");

        if (SystemD3D9Module)
        {
            if (!FreeLibrary(SystemD3D9Module)) {
                HOOKS_LOG()->error("Failed to free system d3d9 library: {0}", LastErrorToString());
            }
            SystemD3D9Module = nullptr;
        }
    }


#pragma region D3D9 exports

    void* WINAPI Direct3DShaderValidatorCreate9() {
        if (!SystemD3D9Funcs.Direct3DShaderValidatorCreate9) {
            HOOKS_LOG()->error("System Direct3DShaderValidatorCreate9 cannot be found");
            return nullptr;
        }
        return SystemD3D9Funcs.Direct3DShaderValidatorCreate9();
    }

    void WINAPI PSGPError() {
        if (!SystemD3D9Funcs.PSGPError) {
            HOOKS_LOG()->error("System PSGPError cannot be found");
            return;
        }
        SystemD3D9Funcs.PSGPError();
    }

    void WINAPI PSGPSampleTexture() {
        if (!SystemD3D9Funcs.PSGPSampleTexture) {
            HOOKS_LOG()->error("System PSGPSampleTexture cannot be found");
            return;
        }
        SystemD3D9Funcs.PSGPSampleTexture();
    }

    int WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName) {
        if (!SystemD3D9Funcs.D3DPERF_BeginEvent) {
            HOOKS_LOG()->error("System D3DPERF_BeginEvent cannot be found");
            return 0;
        }
        return SystemD3D9Funcs.D3DPERF_BeginEvent(col, wszName);
    }

    int WINAPI D3DPERF_EndEvent() {
        if (!SystemD3D9Funcs.D3DPERF_EndEvent) {
            HOOKS_LOG()->error("System D3DPERF_EndEvent cannot be found");
            return 0;
        }
        return SystemD3D9Funcs.D3DPERF_EndEvent();
    }

    DWORD WINAPI D3DPERF_GetStatus() {
        if (!SystemD3D9Funcs.D3DPERF_GetStatus) {
            HOOKS_LOG()->error("System D3DPERF_GetStatus cannot be found");
            return 0;
        }
        return SystemD3D9Funcs.D3DPERF_GetStatus();
    }

    BOOL WINAPI D3DPERF_QueryRepeatFrame() {
        if (!SystemD3D9Funcs.D3DPERF_QueryRepeatFrame) {
            HOOKS_LOG()->error("System D3DPERF_QueryRepeatFrame cannot be found");
            return FALSE;
        }
        return SystemD3D9Funcs.D3DPERF_QueryRepeatFrame();
    }

    void WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName) {
        if (!SystemD3D9Funcs.D3DPERF_SetMarker) {
            HOOKS_LOG()->error("System D3DPERF_SetMarker cannot be found");
            return;
        }
        SystemD3D9Funcs.D3DPERF_SetMarker(col, wszName);
    }

    void WINAPI D3DPERF_SetOptions(DWORD dwOptions) {
        if (!SystemD3D9Funcs.D3DPERF_SetOptions) {
            HOOKS_LOG()->error("System D3DPERF_SetOptions cannot be found");
            return;
        }
        SystemD3D9Funcs.D3DPERF_SetOptions(dwOptions);
    }

    void WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName) {
        if (!SystemD3D9Funcs.D3DPERF_SetRegion) {
            HOOKS_LOG()->error("System D3DPERF_SetRegion cannot be found");
            return;
        }
        SystemD3D9Funcs.D3DPERF_SetRegion(col, wszName);
    }

    void WINAPI DebugSetLevel(LONG level) {
        if (!SystemD3D9Funcs.DebugSetLevel) {
            HOOKS_LOG()->error("System DebugSetLevel cannot be found");
            return;
        }
        SystemD3D9Funcs.DebugSetLevel(level);
    }

    void WINAPI DebugSetMute() {
        if (!SystemD3D9Funcs.DebugSetMute) {
            HOOKS_LOG()->error("System DebugSetMute cannot be found");
            return;
        }
        SystemD3D9Funcs.DebugSetMute();
    }

    void WINAPI Direct3D9EnableMaximizedWindowedModeShim() {
        if (!SystemD3D9Funcs.Direct3D9EnableMaximizedWindowedModeShim) {
            HOOKS_LOG()->error("System Direct3D9EnableMaximizedWindowedModeShim cannot be found");
            return;
        }
        SystemD3D9Funcs.Direct3D9EnableMaximizedWindowedModeShim();
    }

    IDirect3D9* WINAPI Direct3DCreate9(UINT sdkVersion) {
        HOOKS_LOG()->info("Direct3DCreate9 is called");
        SDKVersion = sdkVersion;

        // Create our stuff
        HOOKS_LOG()->info("Proxying D3D9");
        if (!SystemD3D9Funcs.Direct3DCreate9) {
            HOOKS_LOG()->error("System Direct3DCreate cannot be found");
            return nullptr;
        }
        return new LoaderDirect3D9(SystemD3D9Funcs.Direct3DCreate9(sdkVersion));
    }

    IDirect3D9Ex* WINAPI Direct3DCreate9Ex(UINT sdkVersion) {
        // Guild Wars 2 doesn't use this entry point, everything that's called in this function is unfinished.
        // Refer to Direct3DCreate9.

        HOOKS_LOG()->info("Direct3DCreate9Ex is called");
        SDKVersion = sdkVersion;

        // Create our stuff
        HOOKS_LOG()->info("Proxying D3D9Ex");
        if (!SystemD3D9Funcs.Direct3DCreate9Ex) {
            HOOKS_LOG()->error("System Direct3DCreate9Ex cannot be found");
            return nullptr;
        }
        return new LoaderDirect3D9Ex(SystemD3D9Funcs.Direct3DCreate9Ex(sdkVersion));
    }

#pragma endregion

}
