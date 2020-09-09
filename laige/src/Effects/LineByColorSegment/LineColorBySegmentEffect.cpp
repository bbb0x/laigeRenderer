// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "LineColorBySegmentEffect.h"
#include "../InputLayouts/Ip_ColorPosition.h"
#include "../../DxManager.h"


std::string res_matrix = "matrix";




void LineColorBySegmentEffect::Load()
{
	static const D3D11_INPUT_ELEMENT_DESC c_inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // https://docs.microsoft.com/en-gb/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "THICKNESS", 0, DXGI_FORMAT_R32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA, 0} 

};


	LoadVertexShader("vs_LineColorBySegmentEffect.cso", c_inputElementDesc, _countof(c_inputElementDesc));
	LoadGeometryShader("gs_LineColorBySegmentEffect.cso");
	LoadPixelShader("ps_LineColorBySegmentEffect.cso");

	CreateMappedResource<Matrices>(res_matrix, nullptr, ShaderType::Vertex | ShaderType::Geometry);


}

void LineColorBySegmentEffect::Update(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth)
{
	UpdateInputData(data, length, vertexLength, byteWidth, sizeof(lp_ColorPosition));
}

void LineColorBySegmentEffect::Draw()
{
	auto context = DxManager::GetDeviceContext();
	context->IASetInputLayout(m_inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);


	SetShaders();


	UINT strides = sizeof(lp_ColorPosition);
	UINT offsets = 0;

	const auto buf = m_inputBuffer.Get();
	context->IASetVertexBuffers(0, 1,&buf, &strides, &offsets);

	DrawVertices(m_vertexLength, 0);
}

void LineColorBySegmentEffect::SetMatrices(const TwoDMatrix &screenMatrix, const TwoDMatrix* valueMatrix)
{
	Matrices* m = reinterpret_cast<Matrices*> (GetMappedResourceValue(res_matrix));


	if (valueMatrix)
	{
		m->valueMatrix = *valueMatrix;

	}
	else
	{
		m->valueMatrix.SetInvalid();
	}


	m->screenMatrix = screenMatrix;


	SetMappedResourceValue(res_matrix);
}

