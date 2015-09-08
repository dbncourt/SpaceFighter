////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"


Camera::Camera()
{
	this->m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(D3DXVECTOR3 position)
{
	this->m_position = position;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return this->m_position;
}

void Camera::GetPosition(_Out_ D3DXVECTOR3& position)
{
	position = this->m_position;
}

void Camera::SetRotation(D3DXVECTOR3 rotation)
{
	this->m_rotation = rotation;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return this->m_rotation;
}

void Camera::GetRotation(_Out_ D3DXVECTOR3& rotation)
{
	rotation = this->m_rotation;
}

void Camera::Render()
{
	// Setup the vector that points upwards
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// Setup the position of the camera in the world
	D3DXVECTOR3 eye = this->m_position;

	// Setup where the camera is looking by default;
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Set the yaw, pitch and roll rotations in radians
	float pitch = D3DXToRadian(this->m_rotation.x);
	float yaw = D3DXToRadian(this->m_rotation.y);
	float roll = D3DXToRadian(this->m_rotation.z);

	// Create the rotation matrix from the yaw, pitch and roll values
	D3DXMatrixRotationYawPitchRoll(&this->m_viewMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &this->m_viewMatrix);
	D3DXVec3TransformCoord(&up, &up, &this->m_viewMatrix);

	// Translate the rotated camera position to the location of the viewer
	lookAt += eye;

	// Finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&this->m_viewMatrix, &eye, &lookAt, &up);
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return this->m_viewMatrix;
}

void Camera::GetViewMatrix(_Out_ D3DXMATRIX& viewMatrix)
{
	viewMatrix = this->m_viewMatrix;
}