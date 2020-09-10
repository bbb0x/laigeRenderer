// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>

// Manual graphics debugging interface headers https://docs.microsoft.com/en-us/visualstudio/debugger/graphics/walkthrough-capturing-graphics-information-programmatically?view=vs-2019
#include <DXGItype.h>
#include <DXProgrammableCapture.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include "Renderer.h"
#include "Defs.h"
#include "TwoDMatrix.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace DxManager
{
	nodiscard ID3D11DeviceContext* GetDeviceContext() noexcept;
	nodiscard ID3D11Device* GetDevice() noexcept;
	nodiscard Renderer* GetRenderer() noexcept;
	nodiscard const TwoDMatrix& GetScreenMatrix() noexcept;
	void                           SetScreenMatrix(TwoDMatrix& matrix) noexcept;

	void Init();
	void BeginDraw(void* renderTarget, bool isNewSurface, bool enableGraphicsDebugger);
	void EndDraw();
	void Release();

	// Internal no-name namespace for private fields.
	namespace
	{
		ComPtr<IDXGraphicsAnalysis>    m_graphicsAnalysis;
		ComPtr<ID3D11DeviceContext>    m_context;
		ComPtr<ID3D11Device>           m_device;
		ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
		D3D_DRIVER_TYPE                m_driverType;
		D3D_FEATURE_LEVEL              m_featureLevel;
		std::unique_ptr<Renderer>      m_renderer;
		uint32_t                       m_rtWidth;
		uint32_t                       m_rtHeight;
		TwoDMatrix                     m_screenMatrix;
		bool                           m_hasSurface;
		bool                           m_graphicsDebuggerEnabled;

		void InitRenderTarget(void* renderTarget);
		void SetUpViewport();
	}
};