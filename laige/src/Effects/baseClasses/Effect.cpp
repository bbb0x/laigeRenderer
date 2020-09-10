// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#include "Effect.h"
#include "../../DxManager.h"
#include "../../IO.h"

Effect::Effect()
{
	m_vertexLength = 0;
	m_inputLength = 0;
}

Effect::~Effect()
{
	if (m_vertexShader)
		m_vertexShader->Release();

	if (m_geometryShader)
		m_geometryShader->Release();

	if (m_pixelShader)
		m_pixelShader->Release();

	if (m_inputBuffer)
		m_inputBuffer->Release();

	if (m_inputLayout)
		m_inputLayout->Release();
}

void Effect::LoadVertexShader(std::string&& name, const D3D11_INPUT_ELEMENT_DESC* inputDescription, uint32_t inputCount)
{
	auto blob = IO::ReadFile(std::move(name)); // https://stackoverflow.com/questions/35652953/why-c-lvalue-objects-cant-be-bound-to-rvalue-references

	auto c = DxManager::GetDeviceContext();
	auto device = DxManager::GetDevice();
	Utils::ThrowIfFailed(
		device->CreateVertexShader(blob.data(), blob.size(),
			nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

	// Tell DirectX how the vertex-in data looks like.
	Utils::ThrowIfFailed(
		device->CreateInputLayout(inputDescription, inputCount,
			blob.data(), blob.size(),
			m_inputLayout.ReleaseAndGetAddressOf()));
}

void Effect::LoadGeometryShader(std::string&& name)
{
	auto blob = IO::ReadFile(std::move(name));

	auto device = DxManager::GetDevice();
	Utils::ThrowIfFailed(
		device->CreateGeometryShader(blob.data(), blob.size(),
			nullptr, m_geometryShader.ReleaseAndGetAddressOf()));
}

void Effect::LoadPixelShader(std::string&& name)
{
	auto blob = IO::ReadFile(std::move(name));
	auto device = DxManager::GetDevice();
	Utils::ThrowIfFailed(
		device->CreatePixelShader(blob.data(), blob.size(),
			nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
}

void Effect::SetShaders()
{
	auto context = DxManager::GetDeviceContext();

	context->IASetInputLayout(m_inputLayout.Get());

	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

	// geometry shader is optimal, use if exists or clear shader.
	if (m_geometryShader)
		context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	else
		context->GSSetShader(nullptr, nullptr, 0);

	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Set any GPU buffers
	uint32_t i = 0;
	for (auto it = m_mappedResources.begin(); it != m_mappedResources.end(); it++)
	{
		auto value = it->second;
		auto shaderT = value.Types;

		if (shaderT & ShaderType::Vertex)
			SetVSMappedResource(value.Buffer, i);
		if (shaderT & ShaderType::Geometry)
			SetGSMappedResource(value.Buffer, i);
		if (shaderT & ShaderType::Pixel)
			SetPSMappedResource(value.Buffer, i);

		++i;
	}
}

void Effect::UpdateInputData(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth, uint32_t stride)
{
	if (m_inputBuffer)
	{
		m_inputBuffer.Reset();
	}
	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = data;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = stride;

	Utils::ThrowIfFailed(
		DxManager::GetDevice()->CreateBuffer(&bufferDesc, &initialData,
			&m_inputBuffer));

	this->m_inputLength = length;
	this->m_vertexLength = vertexLength;
}

void Effect::SetVSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot)
{
	auto context = DxManager::GetDeviceContext();
	context->VSSetConstantBuffers(slot, 1, buf.GetAddressOf());
}

void Effect::SetGSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot)
{
	auto context = DxManager::GetDeviceContext();
	context->GSSetConstantBuffers(slot, 1, buf.GetAddressOf());
}

void Effect::SetPSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot)
{
	auto context = DxManager::GetDeviceContext();
	context->PSSetConstantBuffers(slot, 1, buf.GetAddressOf());
}

void Effect::SetMappedResourceValue(const std::string& name)
{
	auto context = DxManager::GetDeviceContext();
	MappedResourceEntry ptr = m_mappedResources.find(name)->second;
	context->Unmap(ptr.Buffer.Get(), 0);
}

void* Effect::GetMappedResourceValue(const std::string& name) const
{
	auto context = DxManager::GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	MappedResourceEntry ptr = m_mappedResources.find(name)->second;
	auto result = context->Map(ptr.Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	return mappedResource.pData;
}

void Effect::DrawVertices(uint32_t count, uint32_t start) const
{
	auto context = DxManager::GetDeviceContext();
	context->Draw(count, start);
}