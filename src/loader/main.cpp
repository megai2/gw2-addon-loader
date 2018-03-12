#include "main.h"
#include <list>
#include <stdint.h>
#include <imgui.h>
#include "addons/addons_manager.h"
#include "addons/Addon.h"
#include "hooks/hooks_manager.h"
#include "hooks/LoaderDirect3D9.h"
#include "hooks/LoaderDirect3DDevice9.h"
#include "hooks/MumbleLink.h"
#include "gui/gui_manager.h"
#include "gui/imgui.h"
#include "gui/SettingsWindow.h"
#include "utils/debug.h"
#include "Config.h"
#include "input.h"
#include "log.h"

using namespace std;
using namespace loader;
using namespace loader::utils;


HMODULE dllModule;
WNDPROC BaseWndProc;

// We need this here because of out-of-scope issues
string imGuiConfigFile;

#ifdef _DEBUG
bool imGuiDemoOpen = false;
set<uint_fast8_t> imGuiDemoKeybind { VK_SHIFT, VK_MENU, VK_F1 };
#endif


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ProcessInputMessage(msg, wParam, lParam);

    // Pass event to ImGui
    gui::imgui::ProcessWndProc(msg, wParam, lParam);
    const ImGuiIO io = ImGui::GetIO();

    // Only run these for key down/key up (incl. mouse buttons) events and when ImGui doesn't want to capture the keyboard
    if (!RepeatedPressedKeys() && !io.WantCaptureKeyboard) {
        auto pressedKeys = GetPressedKeys();
        auto settingsKeybind = AppConfig.GetSettingsKeybind();
        if (pressedKeys == settingsKeybind) {
            gui::IsWindowOpen(gui::SettingsWnd) ? gui::CloseWindow(gui::SettingsWnd) : gui::ShowWindow(gui::SettingsWnd);
            return true;
        }

#ifdef _DEBUG
        if (pressedKeys == imGuiDemoKeybind) {
            imGuiDemoOpen = !imGuiDemoOpen;
            return true;
        }
#endif
    }

    // Prevent game from receiving input if ImGui requests capture
    switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
            if (io.WantCaptureMouse) {
                return true;
            }
            break;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            if (io.WantCaptureKeyboard) {
                return true;
            }
            break;
        case WM_CHAR:
            if (io.WantTextInput) {
                return true;
            }
            break;
    }

    // Make sure to show a decent cursor when ImGui has mouse focus
    if ((msg == WM_SETCURSOR || msg == WM_MOUSEMOVE) && io.WantCaptureMouse) {
        if (gui::imgui::UpdateMouseCursor()) {
            return true;
        }
    }

    // Process WndProc to addons
    for (auto it = addons::AddonsList.begin(); it != addons::AddonsList.end(); ++it) {
        if ((*it)->HandleWndProc(hWnd, msg, wParam, lParam)) {
            return true;
        }
    }

    return CallWindowProc(BaseWndProc, hWnd, msg, wParam, lParam);
}


HRESULT PreCreateDevice(IDirect3D9* d3d9, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) {
    // Hook WindowProc
    if (!BaseWndProc) {
        BaseWndProc = (WNDPROC)GetWindowLongPtr(hFocusWindow, GWLP_WNDPROC);
        SetWindowLongPtr(hFocusWindow, GWLP_WNDPROC, (LONG_PTR)&WndProc);
    }

    return D3D_OK;
}

void PostCreateDevice(IDirect3D9* d3d9, IDirect3DDevice9* pDeviceInterface, HWND hFocusWindow) {
    // Hook MumbleLink
    hooks::Gw2MumbleLink.Start();
    
    // Initialize addons
    GetLog()->info("Initializing addons");
    addons::InitializeAddons(hooks::SDKVersion, d3d9, pDeviceInterface);

    // Set up ImGui
    ImGuiIO imio = ImGui::GetIO();
    imGuiConfigFile = AppConfig.GetImGuiConfigPath();
    imio.IniFilename = imGuiConfigFile.c_str();

    gui::imgui::Initialize(dllModule, hFocusWindow, pDeviceInterface);

    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowRounding = 2;
    style->ChildRounding = 0;
    style->FrameRounding = 0;
    style->ScrollbarRounding = 0;
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 0.94f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.05f, 0.00f, 0.98f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.33f, 0.33f, 0.33f, 0.78f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.15f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.19f, 0.15f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.05f, 0.02f, 0.98f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.18f, 0.17f, 0.98f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.05f, 0.02f, 0.98f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.27f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.38f, 0.30f, 0.98f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.80f, 0.77f, 0.64f, 0.98f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.54f, 0.43f, 0.98f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 0.94f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.38f, 0.30f, 0.98f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.54f, 0.43f, 0.98f);
    colors[ImGuiCol_Button] = ImVec4(0.29f, 0.23f, 0.18f, 0.98f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.44f, 0.37f, 0.30f, 0.98f);
    colors[ImGuiCol_Header] = ImVec4(0.29f, 0.23f, 0.18f, 0.98f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.44f, 0.36f, 0.30f, 0.98f);
    colors[ImGuiCol_Separator] = ImVec4(0.29f, 0.23f, 0.18f, 0.98f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.36f, 0.30f, 0.98f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.29f, 0.23f, 0.18f, 0.98f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.36f, 0.30f, 0.98f);
    colors[ImGuiCol_CloseButton] = ImVec4(0.29f, 0.23f, 0.18f, 0.98f);
    colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);
    colors[ImGuiCol_CloseButtonActive] = ImVec4(0.44f, 0.36f, 0.30f, 0.98f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 0.94f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.84f, 0.77f, 0.71f, 0.98f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 0.94f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.84f, 0.77f, 0.71f, 0.98f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.84f, 0.77f, 0.71f, 0.27f);
    colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.58f, 0.50f, 0.43f, 0.98f);

    // Load enabled addons
    GetLog()->info("Loading enabled addons");
    addons::LoadAddons(hFocusWindow);
}


void PreReset(IDirect3DDevice9* pDeviceInterface, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    gui::imgui::InvalidateDeviceObjects();
}

void PostReset(IDirect3DDevice9* pDeviceInterface, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    gui::imgui::CreateDeviceObjects();
}

void PrePresent(IDirect3DDevice9* pDeviceInterface, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
    // Draw ImGui stuff
    gui::imgui::NewFrame();

    gui::Render();
#ifdef _DEBUG
    if (imGuiDemoOpen) {
        ImGui::ShowTestWindow();
    }
#endif

    ImGui::Render();
}


bool WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {
            GetLog()->info("GW2 Addon Loader attached");
            LaunchDebugger();

            dllModule = hModule;
            hooks::InitializeHooks();
            
            hooks::PreCreateDeviceHook = &PreCreateDevice;
            hooks::PostCreateDeviceHook = &PostCreateDevice;

            hooks::PreResetHook = &PreReset;
            hooks::PostResetHook = &PostReset;
            hooks::PrePresentHook = &PrePresent;

            // Make ourselves known by setting an environment variable
            // This makes it easy for addon developers to detect early if we are loaded by GW2 or not
            SetEnvironmentVariable(L"_IsGW2AddonLoaderActive", L"1");

            AppConfig.Initialize();
            addons::RefreshAddonList();
        }
        break;
        case DLL_PROCESS_DETACH: {
            hooks::Gw2MumbleLink.Stop();
            gui::imgui::Shutdown();
            GetLog()->info("Unloading and uninitializing addons");
            addons::UnloadAddons();
            addons::UninitializeAddons();
            GetLog()->info("Uninitializing hooks");
            hooks::UninitializeHooks();
            GetLog()->info("GW2 Addon Loader detached");
        }
        break;
    }
    return true;
}
