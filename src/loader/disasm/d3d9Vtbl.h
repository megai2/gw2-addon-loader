#pragma once
#include <windows.h>
#include <d3d9.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        LPVOID QueryInterface;
        LPVOID AddRef;
        LPVOID Release;
        LPVOID TestCooperativeLevel;
        LPVOID GetAvailableTextureMem;
        LPVOID EvictManagedResources;
        LPVOID GetDirect3D;
        LPVOID GetDeviceCaps;
        LPVOID GetDisplayMode;
        LPVOID GetCreationParameters;
        LPVOID SetCursorProperties;
        LPVOID SetCursorPosition;
        LPVOID ShowCursor;
        LPVOID CreateAdditionalSwapChain;
        LPVOID GetSwapChain;
        LPVOID GetNumberOfSwapChains;
        LPVOID Reset;
        LPVOID Present;
        LPVOID GetBackBuffer;
        LPVOID GetRasterStatus;
        LPVOID SetDialogBoxMode;
        LPVOID SetGammaRamp;
        LPVOID GetGammaRamp;
        LPVOID CreateTexture;
        LPVOID CreateVolumeTexture;
        LPVOID CreateCubeTexture;
        LPVOID CreateVertexBuffer;
        LPVOID CreateIndexBuffer;
        LPVOID CreateRenderTarget;
        LPVOID CreateDepthStencilSurface;
        LPVOID UpdateSurface;
        LPVOID UpdateTexture;
        LPVOID GetRenderTargetData;
        LPVOID GetFrontBufferData;
        LPVOID StretchRect;
        LPVOID ColorFill;
        LPVOID CreateOffscreenPlainSurface;
        LPVOID SetRenderTarget;
        LPVOID GetRenderTarget;
        LPVOID SetDepthStencilSurface;
        LPVOID GetDepthStencilSurface;
        LPVOID BeginScene;
        LPVOID EndScene;
        LPVOID Clear;
        LPVOID SetTransform;
        LPVOID GetTransform;
        LPVOID MultiplyTransform;
        LPVOID SetViewport;
        LPVOID GetViewport;
        LPVOID SetMaterial;
        LPVOID GetMaterial;
        LPVOID SetLight;
        LPVOID GetLight;
        LPVOID LightEnable;
        LPVOID GetLightEnable;
        LPVOID SetClipPlane;
        LPVOID GetClipPlane;
        LPVOID SetRenderState;
        LPVOID GetRenderState;
        LPVOID CreateStateBlock;
        LPVOID BeginStateBlock;
        LPVOID EndStateBlock;
        LPVOID SetClipStatus;
        LPVOID GetClipStatus;
        LPVOID GetTexture;
        LPVOID SetTexture;
        LPVOID GetTextureStageState;
        LPVOID SetTextureStageState;
        LPVOID GetSamplerState;
        LPVOID SetSamplerState;
        LPVOID ValidateDevice;
        LPVOID SetPaletteEntries;
        LPVOID GetPaletteEntries;
        LPVOID SetCurrentTexturePalette;
        LPVOID GetCurrentTexturePalette;
        LPVOID SetScissorRect;
        LPVOID GetScissorRect;
        LPVOID SetSoftwareVertexProcessing;
        LPVOID GetSoftwareVertexProcessing;
        LPVOID SetNPatchMode;
        LPVOID GetNPatchMode;
        LPVOID DrawPrimitive;
        LPVOID DrawIndexedPrimitive;
        LPVOID DrawPrimitiveUP;
        LPVOID DrawIndexedPrimitiveUP;
        LPVOID ProcessVertices;
        LPVOID CreateVertexDeclaration;
        LPVOID SetVertexDeclaration;
        LPVOID GetVertexDeclaration;
        LPVOID SetFVF;
        LPVOID GetFVF;
        LPVOID CreateVertexShader;
        LPVOID SetVertexShader;
        LPVOID GetVertexShader;
        LPVOID SetVertexShaderConstantF;
        LPVOID GetVertexShaderConstantF;
        LPVOID SetVertexShaderConstantI;
        LPVOID GetVertexShaderConstantI;
        LPVOID SetVertexShaderConstantB;
        LPVOID GetVertexShaderConstantB;
        LPVOID SetStreamSource;
        LPVOID GetStreamSource;
        LPVOID SetStreamSourceFreq;
        LPVOID GetStreamSourceFreq;
        LPVOID SetIndices;
        LPVOID GetIndices;
        LPVOID CreatePixelShader;
        LPVOID SetPixelShader;
        LPVOID GetPixelShader;
        LPVOID SetPixelShaderConstantF;
        LPVOID GetPixelShaderConstantF;
        LPVOID SetPixelShaderConstantI;
        LPVOID GetPixelShaderConstantI;
        LPVOID SetPixelShaderConstantB;
        LPVOID GetPixelShaderConstantB;
        LPVOID DrawRectPatch;
        LPVOID DrawTriPatch;
        LPVOID DeletePatch;
        LPVOID CreateQuery;
        LPVOID SetConvolutionMonoKernel;
        LPVOID ComposeRects;
        LPVOID PresentEx;
        LPVOID GetGPUThreadPriority;
        LPVOID SetGPUThreadPriority;
        LPVOID WaitForVBlank;
        LPVOID CheckResourceResidency;
        LPVOID SetMaximumFrameLatency;
        LPVOID GetMaximumFrameLatency;
        LPVOID CheckDeviceState;
        LPVOID CreateRenderTargetEx;
        LPVOID CreateOffscreenPlainSurfaceEx;
        LPVOID CreateDepthStencilSurfaceEx;
        LPVOID ResetEx;
        LPVOID GetDisplayModeEx;
    } D3DDevice9Vtbl;

    D3DDevice9Vtbl GetD3DDevice9Vtbl(const IDirect3DDevice9* const obj);
    void SetD3DDevice9Vtbl(IDirect3DDevice9* const obj, const D3DDevice9Vtbl vtbl);

#ifdef __cplusplus
}
#endif