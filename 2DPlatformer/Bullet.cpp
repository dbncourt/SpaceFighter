////////////////////////////////////////////////////////////////////////////////
// Filename: Bullet.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Bullet.h"

Bullet::Bullet() : GameObject()
{
	this->m_CircleCollider = nullptr;
}

Bullet::Bullet(const Bullet& other)
{
}

Bullet::~Bullet()
{
}

bool Bullet::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;
	this->m_drawCollider = drawCollider;

	result = GameObject::Initialize(device, hwnd, screen, L"Bullet.dds", Bitmap::DimensionType{ 18, 3 }, Bitmap::DimensionType{ 18, 3 }, 1, 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Bullet GameObject.", L"Error", MB_OK);
		return false;
	}

	this->m_Collider = new CircleCollider();
	if (!this->m_Collider)
	{
		return false;
	}

	this->m_CircleCollider = dynamic_cast<CircleCollider*>(this->m_Collider);
	result = this->m_CircleCollider->Initialize(device, hwnd, screen, POINT{ 12, 2 }, 5.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

void Bullet::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
		GameObject::ResetMovementDelayTime();
	}

	this->m_CircleCollider->Frame(GameObject::GetPosition());
}

bool Bullet::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = GameObject::Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	if (this->m_drawCollider)
	{
		result = this->m_CircleCollider->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	return true;
}