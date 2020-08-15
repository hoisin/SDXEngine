#include "stdafx.h"
#include "SDXIndexBuffer.h"


using namespace SDXEngine;


SDXIndexBuffer::SDXIndexBuffer() : m_buffer(nullptr), m_count(0)
{
}

SDXIndexBuffer::~SDXIndexBuffer()
{
	//Release();
}

SDXErrorId SDXIndexBuffer::LoadData(SDXMeshData* meshData, SDXDirectX* inDirectX)
{
	if (meshData == nullptr)
		return SDXErrorId::SDX_ERROR_INDEXBUFFER_MESHDATA_NULL;

	if (inDirectX == nullptr)
		return SDXErrorId::SDX_ERROR_INDEXBUFFER_DIRECTX_NULL;

	ID3D11Device* device = inDirectX->GetDevice().Get();

	if (device == nullptr)
		return SDXErrorId::SDX_ERROR_DEVICE_NOT_CREATED;

	// Create and load index buffer
	CD3D11_BUFFER_DESC iDesc(meshData->GetIndexCount() * sizeof(int), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = meshData->GetIndexData();
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&iDesc, &iData, &m_buffer);
	if (FAILED(result))
		return SDXErrorId::SDX_ERROR_INDEXBUFFER_CREATE_FAILED;

	m_count = meshData->GetIndexCount();
	return SDXErrorId::SDX_ERROR_NONE;
}

ID3D11Buffer * SDXEngine::SDXIndexBuffer::GetBuffer() const
{
	return m_buffer;
}

int SDXEngine::SDXIndexBuffer::GetCount() const
{
	return m_count;
}

void SDXEngine::SDXIndexBuffer::Release()
{
	m_count = 0;
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}
}
