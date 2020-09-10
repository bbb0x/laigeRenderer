// Copyright (c) 2020 Robin Betka. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <string>
#include <map>
#include <wrl.h>
#include <d3d11.h>
#include "../../Utils.h"
#include "../../BoxMath.h"

using namespace std;
using namespace Microsoft::WRL;

enum ShaderType
{
	Vertex = 1,
	Geometry = 2,
	Pixel = 4
};

struct MappedResourceEntry
{
	int Types = 0;
	ComPtr<ID3D11Buffer> Buffer;
};

class Effect
{
protected:
	ComPtr<ID3D11VertexShader>			  m_vertexShader;
	ComPtr<ID3D11GeometryShader>		  m_geometryShader;
	ComPtr<ID3D11PixelShader>		      m_pixelShader;

	ComPtr<ID3D11InputLayout>			  m_inputLayout;
	ComPtr<ID3D11Buffer>				  m_inputBuffer;
	map<std::string, MappedResourceEntry> m_mappedResources;
	uint32_t							  m_vertexLength;
	uint32_t							  m_inputLength;

	Effect();
	~Effect();
	virtual void Load() = 0;

	void DrawVertices(uint32_t count, uint32_t start) const;
	void LoadVertexShader(std::string&& name, const D3D11_INPUT_ELEMENT_DESC* inputDescription, uint32_t  inputCount);
	void LoadGeometryShader(std::string&& name);
	void LoadPixelShader(std::string&& name);
	void UpdateInputData(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth, uint32_t stride);
	void SetShaders();

public:

	virtual void Update(void* data, uint32_t length, uint32_t vertexLength, uint32_t byteWidth) = 0;
	virtual void Draw() = 0;

	void* GetMappedResourceValue(const std::string& name) const;
	void SetMappedResourceValue(const std::string& name);

private:

	void SetVSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot);
	void SetGSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot);
	void SetPSMappedResource(ComPtr<ID3D11Buffer> buf, uint32_t slot);

protected:

	template <typename T>
	void CreateMappedResource(const std::string& name, std::shared_ptr<T> initialValue, int shaderTypes)
	{
		D3D11_BUFFER_DESC bd = { 0 };

		// MUST be multiple of 16 (if USAGE_DYNAMIC) if bind flags constant buffer. Ensure it.
		// Leftover bytes are not relevant but graphics memory simply works like that.
		bd.ByteWidth = BoxMath::RoundUp(sizeof(T), 16);
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.Usage = D3D11_USAGE_DYNAMIC; // or default?
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		auto device = DxManager::GetDevice();

		ComPtr<ID3D11Buffer> buf;
		Utils::ThrowIfFailed(device->CreateBuffer(&bd, nullptr, &buf));

		MappedResourceEntry e;
		e.Buffer = buf;
		e.Types = shaderTypes;
		m_mappedResources[name] = e;

		if (initialValue)
		{
			T* resource = reinterpret_cast<T*>(GetMappedResourceValue(name));
			T value = *initialValue;
			*resource = value;
			SetMappedResourceValue(name);
		}
	}
};
