// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "TriangleTestEffect.h"
#include "../InputLayouts/lp_testTriangle.h"
#include "../../DxManager.h"

void TriangleTestEffect::Load()
{
	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};
	LoadVertexShader("vs_triangletest.cso", s_inputElementDesc, _countof(s_inputElementDesc));
	LoadPixelShader("ps_triangletest.cso");

}

void TriangleTestEffect::Update(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth)
{
	// Create vertex buffer.
	lp_testTriangle s_vertexData[3] =
	{
		{ { 0.0f,   1.0f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
		{ { 0.5f,  -0.5f,  0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
		{ { -0.5f, -0.5f,  0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },   // Left / Blue
		//   { { 0.0f,   0.5f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
		//{ { 1.0f,  -0.5f,  0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
		//{ { -1.0f, -0.5f,  0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }   // Left / Blue
	};
	UpdateInputData(s_vertexData, 3, vertexLength, sizeof(s_vertexData), sizeof(lp_testTriangle));
}

void TriangleTestEffect::Draw() 
{
	auto context = DxManager::GetDeviceContext();
	context->IASetInputLayout(m_inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetShaders();

	UINT strides = sizeof(lp_testTriangle);
	UINT offsets = 0;

	const auto buf = m_inputBuffer.Get();
	context->IASetVertexBuffers(0, 1, &buf, &strides, &offsets);

	context->Draw(3, 0);
}
