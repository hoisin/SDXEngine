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


protected:
	int m_modelID;

	XMFLOAT3 m_position;
	XMFLOAT3 m_prevPosition;

	BoundingBox m_collisionBox;
	// Collision box / sphere
};

