////////////////////////////////////////////////////////////////////////////////
// Filename: Bullet.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Bullet.h"

Bullet::Bullet() : GameObject()
{
}

Bullet::~Bullet()
{
}

bool Bullet::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Bullet.dds", Bitmap::DimensionType{ 18, 3 }, Bitmap::DimensionType{ 18, 3 }, 1, 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Bullet GameObject.", L"Error", MB_OK);
		return false;
	}

	GameObject::SetSphereCollider(GameObject::SphereColliderType{ 10.0f, POINT{ 10, 2 } });

	return true;
}

void Bullet::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
	}
}