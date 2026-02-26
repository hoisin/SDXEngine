#include "stdafx.h"
#include "CObject.h"

CObject::CObject()
	: m_modelID()
	, m_position(0.0f, 0.0f, 0.0f)
	, m_prevPosition(0.0f, 0.0f, 0.0f)
	, m_collisionSphere()
{
}

CObject::~CObject()
{
}

void CObject::SetModelID(const std::string& modelID)
{
	m_modelID = modelID;
}

std::string CObject::GetModelID() const
{
	return m_modelID;
}

void CObject::SetPosition(const XMFLOAT3& position)
{
	m_position = position;
}

XMFLOAT3 CObject::GetPosition() const
{
	return m_position;
}

void CObject::SetPrevPosition(const XMFLOAT3& prevPosition)
{
	m_prevPosition = prevPosition;
}

XMFLOAT3 CObject::GetPrevPosition() const
{
	return m_prevPosition;
}

void CObject::SetCollisionSphere(const BoundingSphere& sphere)
{
	m_collisionSphere = sphere;
}

BoundingSphere CObject::GetCollisionSphere() const
{
	return m_collisionSphere;
}
