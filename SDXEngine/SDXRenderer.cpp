#include "stdafx.h"
#include "SDXGfx.h"
#include "SDXRenderer.h"
#include "SDXDirectXShaderCompiler.h"
#include "SDXRasterState.h"

#include "SDXMeshGenerator.h"

#include "SDXAssetMGR.h"

using namespace SDXEngine;

SDXRenderer::SDXRenderer()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_indexCount = 0;

	m_testFrameCount = 0;
	m_worldViewProj = ConstantBufferStruct();
}

SDXRenderer::~SDXRenderer()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
}

SDXErrorId SDXEngine::SDXRenderer::Initialise(const SDXDirectXInfo & info)
{
	// Info check
	if (info.clientHeight == 0 || info.clientWidth == 0)
		return SDX_ERROR_RENDERER_CLIENTAREA_NOT_SET;

	if (info.hwnd == nullptr)
		return SDX_ERROR_RENDERER_OUTPUTWINDOW_NOT_SET;
		
	SDXErrorId error = SDX_ERROR_NONE;
	error = m_directX.Initialise(info);

	// Setup direct2D stuff
	error = m_direct2D.Initialise(&m_directX);
	if (IsError(error))
		return error;

	return SDX_ERROR_NONE;
}

void SDXEngine::SDXRenderer::BeginDraw()
{
	// Test clear colour
	// Remember to revert to black
	m_directX.ClearRenderTargetView(0.3f, 0.1f, 0.1f);
	m_directX.ClearDepthStencilView();
}

void SDXEngine::SDXRenderer::EndDraw()
{
	// If error, should be logged.
	m_directX.SwapChainPresent();
}

void SDXEngine::SDXRenderer::UpdateProjectionMatrix(const XMFLOAT4X4 & proj)
{
	m_worldViewProj.projection = proj;
}

void SDXEngine::SDXRenderer::UpdateViewMatrix(const XMFLOAT4X4 & view)
{
	m_worldViewProj.view = view;
}

void SDXEngine::SDXRenderer::RenderText(UINT x, UINT y, const std::string & text)
{
	// If error, should be logged.
	m_direct2D.RenderText(x, y, text);
}

// Note:
//
// This code has hard coded crap!!!!!!!!!!
// For testing purposes. REMEMBER TO REFACTOR THIS
//
// This is likely to be ported out to a resource hanlder.
// Since this is resource creation/management
//
SDXErrorId SDXEngine::SDXRenderer::CreateCube()
{
	ID3D11Device* device = m_directX.GetDevice().Get();
	if (!device)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXMeshGenerator generator;
	SDXMeshData* pMesh = generator.GenerateCube(11, SDXVERTEX_TYPE_PNC, 2, XMFLOAT3(0, 1, 0));
	int bytes = pMesh->GetVertexCount() * GetSizeOfVertexType(pMesh->GetVertexType());

	// Create vertex buffer:
	CD3D11_BUFFER_DESC vDesc(
		bytes,
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA  vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = pMesh->GetVertexData();
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(
		&vDesc,
		&vData,
		&m_vertexBuffer
	);

	m_indexCount = pMesh->GetIndexCount();//RRAYSIZE(cubeIndices);

	CD3D11_BUFFER_DESC iDesc(
		pMesh->GetIndexCount() * sizeof(unsigned int),
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = pMesh->GetIndexData();
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(
		&iDesc,
		&iData,
		&m_indexBuffer
	);

	m_fillState;
	m_wireFrame.SetWireFrame(true);
	m_directX.SetRasterState(m_fillState);

	delete pMesh;
	pMesh = nullptr;

	return SDX_ERROR_NONE;
}

// Note:
//
// This code has hard coded crap!!!!!!!!!!
// For testing purposes. REMEMBER TO REFACTOR THIS
//
// This is likely to be ported out to a resource hanlder.
// Since this is resource creation/management
//
SDXErrorId SDXEngine::SDXRenderer::CreateViewAndPerspective()
{
	// Use DirectXMath to create view and perspective matrices.

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, -0.1f, 0.0f, 0.f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	XMStoreFloat4x4(&m_worldViewProj.world, XMMatrixIdentity());

	return SDX_ERROR_NONE;
}

void SDXEngine::SDXRenderer::UpdateTest()
{
	// Rotate the cube 1 degree per frame.
	DirectX::XMStoreFloat4x4(
		&m_worldViewProj.world,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixRotationY(
				DirectX::XMConvertToRadians(
				(float)m_testFrameCount//++
				)
			)
		)
	);

	if (m_testFrameCount == MAXUINT)  m_testFrameCount = 0;
}

// Test function
void SDXEngine::SDXRenderer::RenderCube()
{
	// Use the Direct3D device context to draw.
	ID3D11DeviceContext* context = m_directX.GetContext().Get();

	ID3D11RenderTargetView* renderTarget = m_directX.GetRenderTargetView().Get();
	ID3D11DepthStencilView* depthStencil = m_directX.GetDepthStencilView().Get();

	SDXAssetMGR* pAssetMgr = ASSETMGR->GetInstance();

	std::string cbufferID = "worldViewProj";
	context->UpdateSubresource(
		pAssetMgr->GetCBuffer(cbufferID).Get(),
		0,
		nullptr,
		&m_worldViewProj,
		0,
		0
	);

	//// Set the render target.
	//
	//// Code to swap render targets for different techniques
	//	i.e. deferred rendering or shadow maps
	//
	//context->OMSetRenderTargets(
	//	1,
	//	&renderTarget,
	//	depthStencil
	//);
	
	// Vertex/pixel buffer stuff
	//===================================================================================

	// Set up the IA stage by setting the input topology and layout.
	UINT stride = GetSizeOfVertexType(SDXVERTEX_TYPE_PNC);
	UINT offset = 0;

	context->IASetVertexBuffers(
		0,
		1,
		&m_vertexBuffer,
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
	);

	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	// ============================================================================================

	// Shader stuff
	//
	// Get the shader
	SShader* pShader = pAssetMgr->GetShader("basic_shader");

	// Input layout set
	context->IASetInputLayout(pShader->inputLayout.Get());

	// Set up the vertex shader stage.
	context->VSSetShader(
		pShader->vertexShader.Get(),
		nullptr,
		0
	);

	// Constant buffer/s
	context->VSSetConstantBuffers(
		0,
		1,
		pAssetMgr->GetCBuffer(cbufferID).GetAddressOf()
	);

	// Set up the pixel shader stage.
	context->PSSetShader(
		pShader->pixelShader.Get(),
		nullptr,
		0
	);

	// Calling Draw tells Direct3D to start sending commands to the graphics device.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

void SDXEngine::SDXRenderer::EnableWireFrame(bool bEnable)
{
	if (bEnable)
		m_directX.SetRasterState(m_wireFrame);
	else
		m_directX.SetRasterState(m_fillState);
}

SDXDirectX* SDXEngine::SDXRenderer::GetDirectX()
{
	return &m_directX;
}