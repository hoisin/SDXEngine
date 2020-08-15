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

	m_wireFrame.SetWireFrame(true);

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

void SDXEngine::SDXRenderer::Render(SDXDrawItem* drawItem)
{
	// Use the Direct3D device context to draw.
	ID3D11DeviceContext* context = m_directX.GetContext().Get();

	ID3D11RenderTargetView* renderTarget = m_directX.GetRenderTargetView().Get();
	ID3D11DepthStencilView* depthStencil = m_directX.GetDepthStencilView().Get();

	SDXAssetMGR* pAssetMgr = ASSETMGR->GetInstance();
	std::string cbufferID = "worldViewProj";

	XMMATRIX scale = XMMatrixScaling(drawItem->scale.x, drawItem->scale.y, drawItem->scale.z);
	XMMATRIX trans = XMMatrixTranslation(drawItem->worldPos.x, drawItem->worldPos.y, drawItem->worldPos.z);
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(drawItem->rotation.x), 
		XMConvertToRadians(drawItem->rotation.y), 
		XMConvertToRadians(drawItem->rotation.z)
	);

	auto world = trans * rot * scale;
	world = XMMatrixTranspose(world);
	
	DirectX::XMStoreFloat4x4(
		&m_worldViewProj.world,
		world
	);

	//DirectX::XMStoreFloat4x4(
	//	&m_worldViewProj.invWorldTrans,
	//	XMMatrixTranspose(XMMatrixInverse(nullptr, world))
	//);

	context->UpdateSubresource(
		pAssetMgr->GetCBuffer(cbufferID).Get(),
		0,
		nullptr,
		&m_worldViewProj,
		0,
		0
	);

	// Foreach submeshes in mesh
	for (int mesh = 0; mesh < static_cast<int>(drawItem->mesh->GetTotalSubMeshes()); mesh++)
	{
		// Only 1 sub mesh (testing)
		SDXSubMesh* pSubMesh = drawItem->mesh->GetSubMesh(mesh);

		// Set up the IA stage by setting the input topology and layout.
		UINT stride = GetSizeOfVertexType(pSubMesh->GetVertexBuffer()->GetType());
		UINT offset = 0;

		ID3D11Buffer* vertexBuf = pSubMesh->GetVertexBuffer()->GetBuffer();
		context->IASetVertexBuffers(
			0,
			1,
			&vertexBuf,
			&stride,
			&offset
		);

		ID3D11Buffer* indexBuf = pSubMesh->GetIndexBuffer()->GetBuffer();
		context->IASetIndexBuffer(
			indexBuf,
			DXGI_FORMAT_R32_UINT,
			0
		);

		context->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);

		// Material
		SMaterial mat = pAssetMgr->GetMaterial(pSubMesh->GetMaterialID());

		// Shader stuff
		//
		// Get the shader
		SShader* pShader = pAssetMgr->GetShader(mat.shaderID);

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
			pSubMesh->GetIndexBuffer()->GetCount(),
			0,
			0
		);
	}
}

void SDXEngine::SDXRenderer::Render(const std::list<SDXDrawItem*>& drawList)
{
	// Use the Direct3D device context to draw.
	ID3D11DeviceContext* context = m_directX.GetContext().Get();

	ID3D11RenderTargetView* renderTarget = m_directX.GetRenderTargetView().Get();
	ID3D11DepthStencilView* depthStencil = m_directX.GetDepthStencilView().Get();

	SDXAssetMGR* pAssetMgr = ASSETMGR->GetInstance();
	std::string cbufferID = "worldViewProj";

	for (auto item = drawList.begin(); item != drawList.end(); item++)
	{
		SDXDrawItem* pDrawItem = (*item);
		XMMATRIX scale = XMMatrixScaling(pDrawItem->scale.x, pDrawItem->scale.y, pDrawItem->scale.z);
		XMMATRIX trans = XMMatrixTranslation(pDrawItem->worldPos.x, pDrawItem->worldPos.y, pDrawItem->worldPos.z);
		XMMATRIX rot = XMMatrixRotationRollPitchYaw(
			XMConvertToRadians(pDrawItem->rotation.x),
			XMConvertToRadians(pDrawItem->rotation.y),
			XMConvertToRadians(pDrawItem->rotation.z)
		);

		auto world = trans * rot * scale;
		world = XMMatrixTranspose(world);

		DirectX::XMStoreFloat4x4(
			&m_worldViewProj.world,
			world
		);

		//DirectX::XMStoreFloat4x4(
		//	&m_worldViewProj.invWorldTrans,
		//	XMMatrixTranspose(XMMatrixInverse(nullptr, world))
		//);

		context->UpdateSubresource(
			pAssetMgr->GetCBuffer(cbufferID).Get(),
			0,
			nullptr,
			&m_worldViewProj,
			0,
			0
		);

		// Foreach submeshes in mesh
		for (int mesh = 0; mesh < static_cast<int>(pDrawItem->mesh->GetTotalSubMeshes()); mesh++)
		{
			// Only 1 sub mesh (testing)
			SDXSubMesh* pSubMesh = pDrawItem->mesh->GetSubMesh(mesh);

			// Set up the IA stage by setting the input topology and layout.
			UINT stride = GetSizeOfVertexType(pSubMesh->GetVertexBuffer()->GetType());
			UINT offset = 0;

			ID3D11Buffer* vertexBuf = pSubMesh->GetVertexBuffer()->GetBuffer();
			context->IASetVertexBuffers(
				0,
				1,
				&vertexBuf,
				&stride,
				&offset
			);

			ID3D11Buffer* indexBuf = pSubMesh->GetIndexBuffer()->GetBuffer();
			context->IASetIndexBuffer(
				indexBuf,
				DXGI_FORMAT_R32_UINT,
				0
			);

			context->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
			);

			// Material
			SMaterial mat = pAssetMgr->GetMaterial(pSubMesh->GetMaterialID());

			// Shader stuff
			//
			// Get the shader
			SShader* pShader = pAssetMgr->GetShader(mat.shaderID);

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
				pSubMesh->GetIndexBuffer()->GetCount(),
				0,
				0
			);
		}
	}
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
	//for (int i = 0; i < pAssetMgr->GetMeshCount(); i++)
	//{
		context->UpdateSubresource(
			pAssetMgr->GetCBuffer(cbufferID).Get(),
			0,
			nullptr,
			&m_worldViewProj,
			0,
			0
		);

		// Get mesh
		SDXMesh* pMesh = pAssetMgr->GetMesh("cube_1");
		// Only 1 sub mesh (testing)
		SDXSubMesh* pSubMesh = pMesh->GetSubMesh(0);

		// Set up the IA stage by setting the input topology and layout.
		UINT stride = GetSizeOfVertexType(pSubMesh->GetVertexBuffer()->GetType());
		UINT offset = 0;

		ID3D11Buffer* vertexBuf = pSubMesh->GetVertexBuffer()->GetBuffer();
		context->IASetVertexBuffers(
			0,
			1,
			&vertexBuf,
			&stride,
			&offset
		);

		ID3D11Buffer* indexBuf = pSubMesh->GetIndexBuffer()->GetBuffer();
		context->IASetIndexBuffer(
			indexBuf,
			DXGI_FORMAT_R32_UINT,
			0
		);

		context->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);

		// ============================================================================================

		// Material
		SMaterial mat = pAssetMgr->GetMaterial(pSubMesh->GetMaterialID());
		
		// Shader stuff
		//
		// Get the shader
		SShader* pShader = pAssetMgr->GetShader(mat.shaderID);

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
			pSubMesh->GetIndexBuffer()->GetCount(),
			0,
			0
		);
	//}
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