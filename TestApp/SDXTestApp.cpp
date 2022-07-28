#include "stdafx.h"
#include "SDXTestApp.h"

#include <string>

bool gBool = false;

SDXTestApp::SDXTestApp() : m_bDebugText(false), m_pAssetMgr(nullptr)
{
}


SDXTestApp::~SDXTestApp()
{
	m_pAssetMgr->Close();
}

SDXErrorId SDXTestApp::OnInitialise()
{
	// Initialise directX
	SDXDirectXInfo info;
	info.clientWidth = GetWindowWidth();
	info.clientHeight = GetWindowHeight();
	info.hwnd = GetHwnd();

	SDXErrorId error = m_inputHDLR.Initialise();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = m_renderer.Initialise(info);
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = m_sceneMGR.Initialise(&m_renderer);
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = InitialiseResources();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	m_renderer.CreateViewAndPerspective();

	m_camera.SetupProjection(0.1f, 10000.f, (float)info.clientWidth / (float)info.clientHeight, 70);
	m_camera.SetupView(XMVectorSet(0.0f, 0.0f, -20.0f, 0.f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.f),
		0, 0);

	m_renderer.UpdateProjectionMatrix(m_camera.GetProjectionMatrix());
	m_renderer.UpdateViewMatrix(m_camera.GetViewMatrix());

	m_bDebugText = true;

	return error;
}

void SDXTestApp::OnUpdate(double deltaT)
{
	// Update input
	m_inputHDLR.UpdateInput(deltaT);

	if (m_inputHDLR.GetMouse()->IsButtonDown(SDXMOUSE_LEFT))
		gBool = true;

	if (m_inputHDLR.GetMouse()->IsButtonUp(SDXMOUSE_LEFT))
		gBool = false;

	if (m_inputHDLR.GetKeyboard()->IsKeyDown(VK_ESCAPE))
		StopRun();

	HandleCamera(deltaT);
}

void SDXTestApp::OnDraw(double deltaT)
{
	m_renderer.BeginDraw();

	DrawMyStuff();

	DrawDebugText();

	m_renderer.EndDraw();
}

void SDXTestApp::OnShutDown()
{
}

bool SDXTestApp::OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handleEvent = false;
	handleEvent = m_inputHDLR.HandleMsg(wnd, msg, wParam, lParam);
	return handleEvent;
}

void SDXTestApp::HandleCamera(double deltaT)
{
	// Mouse
	SDXMouse* mouse = m_inputHDLR.GetMouse();
	if (mouse->IsButtonDown(SDXMOUSE_RIGHT) || mouse->IsButtonHeld(SDXMOUSE_RIGHT))
	{
		if (mouse->IsCursorVisible())
			mouse->ShowCursor(false);

		int hAngle = mouse->GetPos().x - mouse->GetButtonDownPos(SDXMOUSE_RIGHT).x;
		int vAngle = mouse->GetPos().y - mouse->GetButtonDownPos(SDXMOUSE_RIGHT).y;

		float rotAmt = 0.1f;

		m_camera.RotatePitch(-vAngle * rotAmt);
		m_camera.RotateYaw(-hAngle * rotAmt);

		// Snap mouse
		mouse->SetCursorPos(mouse->GetButtonDownPos(SDXMOUSE_RIGHT));
	}

	if (mouse->IsButtonUp(SDXMOUSE_RIGHT))
	{
		if (!mouse->IsCursorVisible())
			mouse->ShowCursor(true);
	}

	// Keyboard
	float moveAmt = 0.1f;
	SDXKeyboard* keyboard = m_inputHDLR.GetKeyboard();
	if (keyboard->IsKeyDown('D') || keyboard->IsKeyHeld('D'))
		m_camera.StrafeRight(moveAmt);

	if (keyboard->IsKeyDown('A') || keyboard->IsKeyHeld('A'))
		m_camera.StrafeLeft(moveAmt);

	if (keyboard->IsKeyDown('W') || keyboard->IsKeyHeld('W'))
		m_camera.MoveForward(moveAmt);

	if (keyboard->IsKeyDown('S') || keyboard->IsKeyHeld('S'))
		m_camera.MoveBackward(moveAmt);

	if (keyboard->IsKeyDown(VK_UP))
		m_camera.RotatePitch(0.1f);

	if (keyboard->IsKeyDown(VK_DOWN))
		m_camera.RotatePitch(-0.1f);

	if (keyboard->IsKeyDown(VK_LEFT))
		m_camera.RotateYaw(0.1f);

	if (keyboard->IsKeyDown(VK_RIGHT))
		m_camera.RotateYaw(-0.1f);

	if (keyboard->IsKeyDown('N'))
		m_renderer.EnableWireFrame(false);

	if (keyboard->IsKeyDown('M'))
		m_renderer.EnableWireFrame(true);

	m_camera.Update();

	m_renderer.UpdateProjectionMatrix(m_camera.GetProjectionMatrix());
	m_renderer.UpdateViewMatrix(m_camera.GetViewMatrix());
}

void SDXTestApp::DrawDebugText()
{
	if (m_bDebugText)
	{
		POINT pos = m_inputHDLR.GetMouse()->GetPos();
		POINT prevPos = m_inputHDLR.GetMouse()->GetPrevPos();
		m_renderer.RenderText(0, 0, "Mouse pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y));
	}
}

SDXErrorId SDXTestApp::InitialiseResources()
{
	// Asset MGR
	m_pAssetMgr = ASSETMGR->GetInstance();
	SDXErrorId error = m_pAssetMgr->Initialise(m_renderer.GetDirectX());
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	// Create our shaders
	error = LoadShaders();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = LoadTextures();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = LoadMaterials();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	error = LoadMeshes();
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	// Define the CBuffer for the shader input
	// Map the constant buffer uniform
	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);
	error = m_pAssetMgr->BindShaderConstant("worldViewProj", &cbDesc);

	return error;
}

SDXErrorId SDXTestApp::LoadShaders()
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	D3D11_INPUT_ELEMENT_DESC inTexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// Load basic shader with position and color
	error = m_pAssetMgr->LoadShader("..\\Assets\\Shaders\\basicTex.vs", "..\\Assets\\Shaders\\basicTex.ps", inTexDesc,
		ARRAYSIZE(inTexDesc), "basicTex_shader");
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	D3D11_INPUT_ELEMENT_DESC inDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// Load basic shader with position and color
	error = m_pAssetMgr->LoadShader("..\\Assets\\Shaders\\basic.vs", "..\\Assets\\Shaders\\basic.ps", inDesc,
		ARRAYSIZE(inDesc), "basic_shader");

	return error;
}

SDXErrorId SDXTestApp::LoadTextures()
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	error = m_pAssetMgr->LoadTexture("..\\Assets\\Textures\\wall_outer.bmp", "wallTexture");

	return error;
}

SDXErrorId SDXTestApp::LoadMaterials()
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	SMaterial basicTexMat{};
	basicTexMat.shaderID = "basicTex_shader";
	basicTexMat.diffuseTextureID = "wallTexture";
	error = m_pAssetMgr->AddMaterial("basicWallMaterial", basicTexMat);
	if (error != SDXErrorId::SDX_ERROR_NONE)
	{
		return error;
	}

	SMaterial basicMat = {};
	basicMat.shaderID = "basic_shader";
	error = m_pAssetMgr->AddMaterial("basicMaterial", basicMat);

	return error;
}

SDXErrorId SDXTestApp::LoadMeshes()
{
	SDXErrorId error = SDXErrorId::SDX_ERROR_NONE;

	// Triangle
	auto triGeo = ASSETMGR->GetMeshGenerator()->GenerateTriangle(10, SDXVertexType::SDXVERTEX_TYPE_PC, XMFLOAT3(0.4, 0.2, 0.5));
	triGeo->SetMaterialID("basicMaterial");

	// Quad
	SDXMeshData* pWallQuad = ASSETMGR->GetMeshGenerator()->GenerateQuad(32, 32, SDXVertexType::SDXVERTEX_TYPE_PNT, 2, 2, XMFLOAT3(0.1, 0.3, 0.5));
	pWallQuad->SetMaterialID("basicWallMaterial");

	SDXMeshData* pColorQuad = ASSETMGR->GetMeshGenerator()->GenerateQuad(32, 32, SDXVertexType::SDXVERTEX_TYPE_PC, 2, 2, XMFLOAT3(0.1, 0.5, 0.4));
	pColorQuad->SetMaterialID("basicMaterial");

	// Mesh generation
	std::string triangleID = "triangle";
	SDXMesh* pTriangleMesh;
	ASSETMGR->CreateMesh(triangleID, &pTriangleMesh);
	pTriangleMesh->AddSubMesh(triGeo, ASSETMGR->GetDirectX());
	delete triGeo;

	std::string wallQuadID = "wall_quad";
	SDXMesh* pWallMesh;
	ASSETMGR->CreateMesh(wallQuadID, &pWallMesh);
	pWallMesh->AddSubMesh(pWallQuad, ASSETMGR->GetDirectX());
	delete pWallQuad;

	std::string wallColorID = "color_quad";
	SDXMesh* pColQuadMesh;
	ASSETMGR->CreateMesh(wallColorID, &pColQuadMesh);
	pColQuadMesh->AddSubMesh(pColorQuad, ASSETMGR->GetDirectX());
	delete pColorQuad;

	return error;
}

void SDXTestApp::DrawMyStuff()
{
	m_sceneMGR.ClearRenderList();

	m_sceneMGR.AddRenderItem(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), ASSETMGR->GetMesh("wall_quad"));
	
	m_sceneMGR.Render();
}