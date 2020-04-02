#include "stdafx.h"
#include "SDXApp.h"

#include <string>

bool gBool = false;

SDXApp::SDXApp() : m_bDebugText(false), m_pAssetMgr(nullptr)
{
}


SDXApp::~SDXApp()
{
	m_pAssetMgr->Close();
}

SDXErrorId SDXApp::OnInitialise()
{
	// Initialise directX
	SDXDirectXInfo info;
	info.clientWidth = GetWindowWidth();
	info.clientHeight = GetWindowHeight();
	info.hwnd = GetHwnd();

	SDXErrorId error = m_inputHDLR.Initialise();
	if (error != SDX_ERROR_NONE)
	{
		return error;
	}

	error = m_renderer.Initialise(info);

	// Asset MGR
	m_pAssetMgr = ASSETMGR->GetInstance();
	error = m_pAssetMgr->Initialise(m_renderer.GetDirectX());
	if (error != SDX_ERROR_NONE)
	{
		return error;
	}

	// Resource creation
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC inDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	// Load shader
	error = m_pAssetMgr->LoadShader("..\\Assets\\Shaders\\dirPNC.vs", "..\\Assets\\Shaders\\dirPNC.ps", inDesc,
		ARRAYSIZE(inDesc), "basic_shader");

	// Define the CBuffer
	// Map the constant buffer uniform
	CD3D11_BUFFER_DESC cbDesc(
		sizeof(ConstantBufferStruct),
		D3D11_BIND_CONSTANT_BUFFER
	);
	error = m_pAssetMgr->BindShaderConstant("worldViewProj", &cbDesc);

	// Material
	SMaterial basicMat = {};
	basicMat.shaderID = "basic_shader";
	m_pAssetMgr->AddMaterial("basicMaterial", basicMat);

	// Create the cube mesh data.
	SDXMeshData* pMeshData = m_pAssetMgr->GetMeshGenerator()->GenerateCube(11, SDXVERTEX_TYPE_PNC, 2, XMFLOAT3(0, 1, 0));
	int bytes = pMeshData->GetVertexCount() * GetSizeOfVertexType(pMeshData->GetVertexType());
	pMeshData->SetMaterialID("basicMaterial");
	
	// Create mesh
	SDXMesh* pMesh = {};
	m_pAssetMgr->CreateMesh("cube_1", &pMesh);
	pMesh->AddSubMesh(pMeshData, m_renderer.GetDirectX());
	

	m_renderer.CreateViewAndPerspective();
	//m_renderer.CreateCube();

	m_camera.SetupProjection(0.1f, 100.f, (float)info.clientWidth / (float)info.clientHeight, 70);
	m_camera.SetupView(XMVectorSet(0.0f, 0.7f, -10.5f, 0.f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.f),
		0, 0);

	m_renderer.UpdateProjectionMatrix(m_camera.GetProjectionMatrix());
	m_renderer.UpdateViewMatrix(m_camera.GetViewMatrix());

	// Loading data XML

	// Load model mesh data

	// Vertex/index buffer creation and upload

	// Mesh generation

	m_bDebugText = true;

	return error;
}

SDXErrorId SDXApp::OnUpdate(double deltaT)
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

	return SDX_ERROR_NONE;
}

SDXErrorId SDXApp::OnDraw(double deltaT)
{
	m_renderer.BeginDraw();

	m_renderer.RenderCube();

	DrawDebugText();

	m_renderer.EndDraw();

	return SDX_ERROR_NONE;
}

void SDXApp::OnShutDown()
{
}

bool SDXApp::OnEvent(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handleEvent = false;
	handleEvent = m_inputHDLR.HandleMsg(wnd, msg, wParam, lParam);
	return handleEvent;
}

void SDXApp::HandleCamera(double deltaT)
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
	float moveAmt = 0.01f;
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

void SDXApp::DrawDebugText()
{
	if (m_bDebugText)
	{
		POINT pos = m_inputHDLR.GetMouse()->GetPos();
		POINT prevPos = m_inputHDLR.GetMouse()->GetPrevPos();
		m_renderer.RenderText(0, 0, "Mouse pos: " + std::to_string(pos.x) + "," + std::to_string(pos.y));
	}
}
