#include "stdafx.h"
#include "SDXVertexBuffer.h"

using namespace SDXEngine;

SDXVertexBuffer::SDXVertexBuffer() : m_buffer(nullptr), m_count(0), m_type(SDXVERTEX_TYPE_UNKNOWN)
{
}

SDXVertexBuffer::~SDXVertexBuffer()
{
	//Release();
}

SDXErrorId SDXEngine::SDXVertexBuffer::LoadData(SDXMeshData * meshData, SDXDirectX* inDirectX)
{
	if (inDirectX == nullptr)
		return SDX_ERROR_VERTEXBUFFER_DIRECTX_NULL;

	// No data error
	if (meshData == nullptr)
		return SDX_ERROR_VERTEXBUFFER_MESHDATA_NULL;

	// Existing buffer error
	if (m_buffer)
		return SDX_ERROR_VERTEXBUFFER_ALREADY_LOADED;
		
	// Unknown vertex type
	if (meshData->GetVertexType() == SDXVERTEX_TYPE_UNKNOWN)
		return SDX_ERROR_VERTEXBUFFER_UNKNOWN_VERTEXTYPE;

	ID3D11Device* device = inDirectX->GetDevice().Get();

	if (device == nullptr)
		return SDX_ERROR_DEVICE_NOT_CREATED;
	
	// Create and load vertex buffer data
	CD3D11_BUFFER_DESC vDesc(meshData->GetVertexCount() * GetSizeOfVertexType(meshData->GetVertexType()), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = meshData->GetVertexData();
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&vDesc, &vData, &m_buffer);
	if (FAILED(result))
		return SDX_ERROR_VERTEXBUFFER_CREATE_FAILED;

	m_type = meshData->GetVertexType();
	m_count = meshData->GetVertexCount();

	// No error
	return SDX_ERROR_NONE;
}

ID3D11Buffer* SDXEngine::SDXVertexBuffer::GetBuffer()
{
	return m_buffer;
}

int SDXEngine::SDXVertexBuffer::GetCount() const
{
	return m_count;
}

SDXVertexType SDXEngine::SDXVertexBuffer::GetType() const
{
	return m_type;
}

void SDXEngine::SDXVertexBuffer::Release()
{
	m_count = 0;
	m_type = SDXVERTEX_TYPE_UNKNOWN;

	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = nullptr;
	}
}
