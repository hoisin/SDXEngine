//--------------------------------------------------------------------------
//
// First-person camera
// Movement commands are 'queued', they don't apply instantly.
// The update method needs to be called in order to apply the movement
//  operations
//
//--------------------------------------------------------------------------
#pragma once

#include <DirectXMath.h>

namespace SDXEngine
{
	class SDXCameraFP
	{
		enum SDXCameraDirection
		{
			SDXCAMERA_DIR_FORWARD,
			SDXCAMERA_DIR_BACKWARD,
			SDXCAMERA_DIR_LEFT,
			SDXCAMERA_DIR_RIGHT,
			SDXCAMERA_TOTAL_DIR
		};

	public:
		SDXCameraFP();
		~SDXCameraFP();

		void SetupProjection(float zNear, float zFar, float aspectRatio,
			float fov);

		void SetupView(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& up,
			float hAngle, float vAngle);

		// Movement commands
		void MoveForward(float amt);
		void MoveBackward(float amt);
		void StrafeLeft(float amt);
		void StrafeRight(float amt);

		void RotatePitch(float degrees);
		void RotateYaw(float degrees);

		// This method updates the camera where it actually actions
		// any of the movement commands that may have been called.
		// Reason is that depending on what commmands has been actioned,
		// they need to be executed in a particular order.
		void Update();

		DirectX::XMFLOAT4X4 GetProjectionMatrix() const;
		DirectX::XMFLOAT4X4 GetViewMatrix() const;

	private:
		float ResetAngle180(float angle);
		float RestrictAngle90(float angle);

	private:
		// View stuff
		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_look;
		DirectX::XMVECTOR m_up;

		DirectX::XMVECTOR m_prevPosition;
		DirectX::XMVECTOR m_prevLook;

		float m_angleH;
		float m_angleV;
		float m_prevAngleH;
		float m_prevAngleV;

		// Projection stuff
		float m_zNear;
		float m_zFar;
		float m_aspectRatio;
		float m_fov;

		bool m_direction[SDXCAMERA_TOTAL_DIR];
		float m_directionAmt[SDXCAMERA_TOTAL_DIR];
	};

}