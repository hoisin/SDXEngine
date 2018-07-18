#include "stdafx.h"
#include "SDXGfx.h"
#include "SDXRenderer.h"
#include "SDXDirectXShaderCompiler.h"
#include "SDXRasterState.h"

#include "SDXMeshGenerator.h"

using namespace SDXEngine;

SDXRenderer::SDXRenderer()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_inputLayout = nullptr;
	m_constantBuffer = nullptr;

	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_indexCount = 0;

	m_testFrameCount = 0;
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

	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = nullptr;
	}

	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

SDXErrorId SDXEngine::SDXRenderer::Initialise(const SDXDirectXInfo & info)
{
	// Info check
	if (info.clientHeight == 0 || info.clientWidth == 0)
		return SDX_ERROR_RENDERER_CLIENTAREA_NOT_SET;

	if (info.hwnd == nullptr)
		return SDX_ERROR_RENDERER_OUTPUTWINDOW_NOT_SET;
		
	// Create device
	SDXErrorId error = m_directX.CreateDevice();
	if (error != SDX_ERROR_NONE)
		return error;

	// Setup directX
	m_directX.SetClientArea(info.clientWidth, info.clientHeight);
	m_directX.SetOutputWindow(info.hwnd);
	m_directX.SetRefreshRate(info.refreshRate);
	m_directX.SetWindowed(info.windowed);

	// Check MSAA quality and set
	UINT quality = 0;
	error = m_directX.Check4XMSAAQuality(quality);
	if (error != SDX_ERROR_NONE)
		return error;

	m_directX.SetUse4XMSAA(info.useMsaa, quality);

	// Create the swap chain
	error = m_directX.CreateSwapChain();
	if (error != SDX_ERROR_NONE)
		return error;

	// Create the render target view
	error = m_directX.CreateRenderTargetView();
	if (error != SDX_ERROR_NONE)
		return error;

	// Create the depth/stencil view
	error = m_directX.CreateDepthStencilBufferView();
	if (error != SDX_ERROR_NONE)
		return error;

	// Bind the output merger
	error = m_directX.BindOutputMerger();
	if (error != SDX_ERROR_NONE)
		return error;

	// Setup direct2D stuff
	m_direct2D.SetSDXDirectX(&m_directX);
	error = m_direct2D.CreateDevice();
	if (error != SDX_ERROR_NONE)
		return error;

	error = m_direct2D.CreateBitmapRenderTarget();
	if (error != SDX_ERROR_NONE)
		return error;

	error = m_direct2D.InitialiseTextFormats();
	if (error != SDX_ERROR_NONE)
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
SDXErrorId SDXEngine::SDXRenderer::CreateShaders()
{
	SDXErrorId error = CreateVertexShader();
	if (error != SDX_ERROR_NONE)
		return error;

	error = CreatePixelShader();
	if (error != SDX_ERROR_NONE)
		return error;

	error = BindConstants();
	if (error != SDX_ERROR_NONE)
		return error;

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
SDXErrorId SDXEngine::SDXRenderer::CreateCube()
{
	ID3D11Device* device = m_directX.GetDevice();
	if (!device)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXMeshGenerator generator;
	SDXMeshData* pMesh = generator.GenerateCube(11, SDXVERTEX_TYPE_PC, 50, XMFLOAT3(0, 1, 0));
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
	ID3D11DeviceContext* context = m_directX.GetContext();

	ID3D11RenderTargetView* renderTarget = m_directX.GetRenderTargetView();
	ID3D11DepthStencilView* depthStencil = m_directX.GetDepthStencilView();

	context->UpdateSubresource(
		m_constantBuffer,
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

	// Set up the IA stage by setting the input topology and layout.
	UINT stride = GetSizeOfVertexType(SDXVERTEX_TYPE_PC);
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

	context->IASetInputLayout(m_inputLayout);

	// Set up the vertex shader stage.
	context->VSSetShader(
		m_vertexShader,
		nullptr,
		0
	);

	context->VSSetConstantBuffers(
		0,
		1,
		&m_constantBuffer
	);

	// Set up the pixel shader stage.
	context->PSSetShader(
		m_pixelShader,
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

SDXErrorId SDXEngine::SDXRenderer::CreateVertexShader()
{
	ID3D11Device* device = m_directX.GetDevice();

	if (!device)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXDirectXShaderCompiler compiler;
	ID3DBlob* shaderBlob = nullptr;
	// Attempt to compile the vertex shader
	SDXErrorId error = compiler.CompileShader("..\\Assets\\Shaders\\basic.vs",
		"Main", "vs_5_0", &shaderBlob);

	// If failed
	if (error != SDX_ERROR_NONE)
	{
		if (shaderBlob)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}
		return SDX_ERROR_RENDERER_VERTEXSHADER_CREATE_FAILED;
	}

	// Attempt to create the vertex shader
	HRESULT result = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		nullptr, &m_vertexShader);

	if (FAILED(result))
	{
		if (shaderBlob)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}
		return SDX_ERROR_RENDERER_VERTEXSHADER_CREATE_FAILED;
	}

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC inDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Attempt to create the input layout
	result = device->CreateInputLayout(inDesc, ARRAYSIZE(inDesc),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_inputLayout);

	if (FAILED(result))
	{
		if (shaderBlob)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}
		return SDX_ERROR_RENDERER_INPUTLAYOUT_CREATE_FAILED;
	}

	// Finished with shaderBlob for vertex buffer
	shaderBlob->Release();
	shaderBlob = nullptr;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXRenderer::CreatePixelShader()
{
	ID3D11Device* device = m_directX.GetDevice();

	if (!device)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXDirectXShaderCompiler compiler;
	ID3DBlob* shaderBlob = nullptr;
	// Attempt to compile the vertex shader
	SDXErrorId error = compiler.CompileShader("..\\Assets\\Shaders\\basic.ps",
		"Main", "ps_5_0", &shaderBlob);

	// If failed
	if (error != SDX_ERROR_NONE)
	{
		if (shaderBlob)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}
		return SDX_ERROR_RENDERER_PIXELSHADER_CREATE_FAILED;
	}

	HRESULT result = device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), nullptr, &m_pixelShader);

	if (FAILED(result))
	{
		if (shaderBlob)
		{
			shaderBlob->Release();
			shaderBlob = nullptr;
		}
		return SDX_ERROR_RENDERER_PIXELSHADER_CREATE_FAILED;
	}

	// Finished with the compiled shader blob
	shaderBlob->Release();
	shaderBlob = nullptr;

	return SDX_ERROR_NONE;
}

SDXErrorId SDXEngine::SDXRenderer::BindConstants()
{
	ID3D11Device* device = m_directX.GetDevice();

	if (!device)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	// Map the constant buffer uniform
	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);

	HRESULT result = device->CreateBuffer(
		&cbDesc,
		nullptr,
		&m_constantBuffer);

	if (FAILED(result))
		return SDX_ERROR_RENDERER_CONSTANTBUFFER_BIND_FAILED;

	return SDX_ERROR_NONE;
}
