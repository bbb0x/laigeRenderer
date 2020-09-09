// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "DxManager.h"
#include "Utils.h"

namespace DxManager
{

    nodiscard ID3D11DeviceContext* GetDeviceContext() noexcept
    {
        return m_context.Get();
    }

    nodiscard ID3D11Device* GetDevice() noexcept
    {
        return m_device.Get();
    }

    nodiscard Renderer* GetRenderer() noexcept
    {
        return m_renderer.get();
    }

    nodiscard const TwoDMatrix& GetScreenMatrix() noexcept
    {
        return m_screenMatrix;
    }

    void  SetScreenMatrix(TwoDMatrix& matrix) noexcept
    {
        m_screenMatrix = matrix;
    }

    void Init()
    {
       // Create DX11 device but min feature level 10.
        m_hasSurface = false;
        UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        // DX10 or 11 devices are suitable
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        HRESULT hr = S_OK;
        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
        {
            hr = D3D11CreateDevice(NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_context);

            if (SUCCEEDED(hr))
            {
                m_driverType = driverTypes[driverTypeIndex];
                break;
            }
        }

        if (FAILED(hr))
        {
            Utils::Throw(hr);
        }
      
        // See if graphics debugger interface is connected.
        HRESULT debugInterfaceRes = DXGIGetDebugInterface1(0, __uuidof(m_graphicsAnalysis), reinterpret_cast<void**>(m_graphicsAnalysis.GetAddressOf()));
        if (FAILED(debugInterfaceRes))
        {
            // There is no graphics debugger connected. Not an issue, no action needed.
        }

        m_renderer = make_unique<Renderer>();

    }



    void BeginDraw(void* renderTarget, bool isNewSurface, bool enableGraphicsDebugger)
    {
 
        // If we've gotten a new Surface, need to initialize the renderTarget.
        // One of the times that this happens is on a resize.

        if (isNewSurface || !m_hasSurface)
        {
            m_context->OMSetRenderTargets(0, NULL, NULL);
            InitRenderTarget(renderTarget);
            m_hasSurface = true;
        }


        static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_context->ClearRenderTargetView(m_pRenderTargetView.Get(), ClearColor);
      
    
        // Capture if graphics debugger is connected (and enabled).
        if (enableGraphicsDebugger && m_graphicsAnalysis)
        {
            m_graphicsAnalysis->BeginCapture();
            m_graphicsDebuggerEnabled = true;
        }

    }

    void EndDraw()
    {
        // End capture (if enabled) and end drawing.
        if (m_graphicsDebuggerEnabled && m_graphicsAnalysis)
        {
            m_graphicsAnalysis->EndCapture();
            m_graphicsDebuggerEnabled = false;
        }

        m_context->Flush();

    }


    void Release()
    {
        m_context->Release();
        m_device->Release();
        if(m_graphicsAnalysis)
        m_graphicsAnalysis->Release();
        m_pRenderTargetView->Release();    
    }
  

    namespace
    {

        void InitRenderTarget(void* renderTarget)
        {
            IUnknown* pUnk = (IUnknown*)renderTarget;

            IDXGIResource* pDXGIResource;
            Utils::ThrowIfFailed(pUnk->QueryInterface(__uuidof(IDXGIResource), (void**)&pDXGIResource));

            HANDLE sharedHandle;
            Utils::ThrowIfFailed(pDXGIResource->GetSharedHandle(&sharedHandle));
    
            pDXGIResource->Release();

            IUnknown* tempResource11;
            Utils::ThrowIfFailed(m_device->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&tempResource11)));
       
            ID3D11Texture2D* pOutputResource;
            Utils::ThrowIfFailed(tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&pOutputResource)));
       
            tempResource11->Release();

            D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
            rtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtDesc.Texture2D.MipSlice = 0;

            Utils::ThrowIfFailed(m_device->CreateRenderTargetView(pOutputResource, &rtDesc, &m_pRenderTargetView));

            D3D11_TEXTURE2D_DESC outputResourceDesc;
            pOutputResource->GetDesc(&outputResourceDesc);


            if (outputResourceDesc.Width != m_rtWidth || outputResourceDesc.Height != m_rtHeight)
            {
                m_rtWidth = outputResourceDesc.Width;
                m_rtHeight = outputResourceDesc.Height;

                SetUpViewport();
            }


            auto n = m_pRenderTargetView.Get();
            const auto ptr = &n;
            m_context->OMSetRenderTargets(1, ptr, NULL);

            if (NULL != pOutputResource)
            {
                pOutputResource->Release();
            }

        }
        void SetUpViewport()
        {
            // Setup the viewport
            D3D11_VIEWPORT vp;
            vp.Width = (float)m_rtWidth;
            vp.Height = (float)m_rtHeight;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            m_context->RSSetViewports(1, &vp);

        }
    }


}


