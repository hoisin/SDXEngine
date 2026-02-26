/*
	Base class object
*/
#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

class CObject
{
public:
	CObject();
	virtual ~CObject();

	void SetModelID(const std::string& modelID);
	std::string GetModelID() const;

	void SetPosition(const XMFLOAT3& position);
	XMFLOAT3 GetPosition() const;

	void SetPrevPosition(const XMFLOAT3& prevPosition);
	XMFLOAT3 GetPrevPosition() const;

	void SetCollisionSphere(const BoundingSphere& sphere);
	BoundingSphere GetCollisionSphere() const;

protected:
	std::string m_modelID;

	XMFLOAT3 m_position;
	XMFLOAT3 m_prevPosition;

	BoundingSphere m_collisionSphere;
	// Collision box / sphere
};

