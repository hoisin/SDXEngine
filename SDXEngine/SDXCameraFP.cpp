#include "stdafx.h"
#include "SDXCameraFP.h"

using namespace SDXEngine;
using namespace DirectX;

SDXCameraFP::SDXCameraFP() : m_angleH(0.0f), m_angleV(0.0f), m_prevAngleH(0.0f), m_prevAngleV(0.0f), m_zNear(0), 
	m_zFar(0), m_aspectRatio(0), m_fov(0)
{
	m_position = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	m_look = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	m_up = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	m_prevPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	m_prevLook = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	for (int i = 0; i < SDXCAMERA_TOTAL_DIR; i++)
	{
		m_direction[i] = false;
		m_directionAmt[i] = 0.0;
	}
}


SDXCameraFP::~SDXCameraFP()
{
}

void SDXEngine::SDXCameraFP::SetupProjection(float zNear, float zFar, float aspectRatio, float fov)
{
	m_zNear = zNear;
	m_zFar = zFar;
	m_aspectRatio = aspectRatio;
	m_fov = fov;
}

void SDXEngine::SDXCameraFP::SetupView(const XMVECTOR & position, const XMVECTOR & up,
	float hAngle, float vAngle)
{
	m_position = position;
	m_up = up;

	//convert angles to radians
	m_angleH = (float)(hAngle * (XM_PI / 180));
	m_angleV = (float)(vAngle * (XM_PI / 180));
	m_prevAngleH = m_angleH;
	m_prevAngleV = m_angleV;

	//calculate interest
	m_look.m128_f32[0] = m_position.m128_f32[0] - sin(m_angleH)*cos(m_angleV);
	m_look.m128_f32[1] = m_position.m128_f32[1] - sin(m_angleV);
	m_look.m128_f32[2] = m_position.m128_f32[2] + cos(m_angleH)*cos(m_angleV);
}

void SDXEngine::SDXCameraFP::MoveForward(float amt)
{
	m_direction[SDXCAMERA_DIR_FORWARD] = true;
	m_directionAmt[SDXCAMERA_DIR_FORWARD] = amt;
}

void SDXEngine::SDXCameraFP::MoveBackward(float amt)
{
	m_direction[SDXCAMERA_DIR_BACKWARD] = true;
	m_directionAmt[SDXCAMERA_DIR_BACKWARD] = amt * -1;
}

void SDXEngine::SDXCameraFP::StrafeLeft(float amt)
{
	m_direction[SDXCAMERA_DIR_LEFT] = true;
	m_directionAmt[SDXCAMERA_DIR_LEFT] = amt * -1;
}

void SDXEngine::SDXCameraFP::StrafeRight(float amt)
{
	m_direction[SDXCAMERA_DIR_RIGHT] = true;
	m_directionAmt[SDXCAMERA_DIR_RIGHT] = amt;
}

void SDXEngine::SDXCameraFP::RotatePitch(float degrees)
{
	m_prevAngleV = m_angleV;
	m_angleV -= (float)(degrees * (XM_PI / 180));
	m_angleV = RestrictAngle90(m_angleV);
}

void SDXEngine::SDXCameraFP::RotateYaw(float degrees)
{
	m_prevAngleH = m_angleH;
	m_angleH += degrees * (XM_PI / 180.f);
	m_angleH = ResetAngle180(m_angleH);
}

void SDXEngine::SDXCameraFP::Update()
{
	// Update previous values
	m_prevPosition = m_position;
	m_prevLook = m_look;
	m_prevAngleH = m_angleH;
	m_prevAngleV = m_angleV;

	// Rotate first
	m_look.m128_f32[0] = m_position.m128_f32[0] - sin(m_angleH)*cos(m_angleV);
	m_look.m128_f32[1] = m_position.m128_f32[1] - sin(m_angleV);
	m_look.m128_f32[2] = m_position.m128_f32[2] + cos(m_angleH)*cos(m_angleV);

	float viewDirX = m_look.m128_f32[0] - m_position.m128_f32[0];
	float viewDirY = m_look.m128_f32[1] - m_position.m128_f32[1];
	float viewDirZ = m_look.m128_f32[2] - m_position.m128_f32[2];

	// Now do translations
	if (m_direction[SDXCAMERA_DIR_FORWARD])
	{
		m_position.m128_f32[0] += m_directionAmt[SDXCAMERA_DIR_FORWARD] * viewDirX;
		m_position.m128_f32[1] += m_directionAmt[SDXCAMERA_DIR_FORWARD] * viewDirY;	//remove this to prevent player moving up when facing up
		m_position.m128_f32[2] += m_directionAmt[SDXCAMERA_DIR_FORWARD] * viewDirZ;
	}

	if (m_direction[SDXCAMERA_DIR_BACKWARD])
	{
		m_position.m128_f32[0] += m_directionAmt[SDXCAMERA_DIR_BACKWARD] * viewDirX;
		m_position.m128_f32[1] += m_directionAmt[SDXCAMERA_DIR_BACKWARD] * viewDirY;	//remove this to prevent player moving up when facing up
		m_position.m128_f32[2] += m_directionAmt[SDXCAMERA_DIR_BACKWARD] * viewDirZ;
	}

	if (m_direction[SDXCAMERA_DIR_LEFT])
	{
		m_position.m128_f32[0] += cos(m_angleH)*m_directionAmt[SDXCAMERA_DIR_LEFT];
		m_position.m128_f32[2] += sin(m_angleH)*m_directionAmt[SDXCAMERA_DIR_LEFT];
	}

	if (m_direction[SDXCAMERA_DIR_RIGHT])
	{
		m_position.m128_f32[0] += cos(m_angleH)*m_directionAmt[SDXCAMERA_DIR_RIGHT];
		m_position.m128_f32[2] += sin(m_angleH)*m_directionAmt[SDXCAMERA_DIR_RIGHT];
	}

	// Update look after translations
	m_look.m128_f32[0] = m_position.m128_f32[0] - sin(m_angleH)*cos(m_angleV);
	m_look.m128_f32[1] = m_position.m128_f32[1] - sin(m_angleV);
	m_look.m128_f32[2] = m_position.m128_f32[2] + cos(m_angleH)*cos(m_angleV);

	// Reset
	for (int i = 0; i < SDXCAMERA_TOTAL_DIR; i++)
	{
		m_direction[i] = false;
		m_directionAmt[i] = 0;
	}
}

DirectX::XMFLOAT4X4 SDXEngine::SDXCameraFP::GetProjectionMatrix() const
{
	// Generate project matrix
	XMFLOAT4X4 proj;
	DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(m_fov),
			m_aspectRatio,
			m_zNear,
			m_zFar
			)
		)
	);

	return proj;
}

DirectX::XMFLOAT4X4 SDXEngine::SDXCameraFP::GetViewMatrix() const
{
	// Generate view matrix
	// 'LH' generates a row major matrix.
	// For the shader we require a column major matrix so transpose
	XMFLOAT4X4 proj;
	DirectX::XMStoreFloat4x4(&proj,
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixLookAtLH(
				m_position,
				m_look,
				m_up
			)
		)
	);

	return proj;
}

float SDXEngine::SDXCameraFP::ResetAngle180(float angle)
{
	float temp = angle;
	if (temp > (180 * (XM_PI / 180)))
		temp += (-360 * (XM_PI / 180));

	if (angle < (-180 * (XM_PI / 180)))
		temp += (360 * (XM_PI / 180));

	return temp;
}

float SDXEngine::SDXCameraFP::RestrictAngle90(float angle)
{
	float temp = angle;

	float maxBound = 89.99f * (XM_PI / 180);
	float minBound = -89.99f * (XM_PI / 180);

	if (angle > maxBound)
		temp = maxBound;

	if (angle < minBound)
		temp = minBound;

	return temp;
}
