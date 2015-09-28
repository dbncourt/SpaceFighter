////////////////////////////////////////////////////////////////////////////////
// Filename: Exhaust.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Exhaust.h"

Exhaust::Exhaust() : GameObject()
{
}

Exhaust::Exhaust(const Exhaust& other)
{
}

Exhaust::~Exhaust()
{
}

bool Exhaust::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Exhaust.dds", Bitmap::DimensionType{ 3, 3 }, Bitmap::DimensionType{ 3, 3 }, 1, 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Exhaust GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Exhaust::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
		GameObject::ResetMovementDelayTime();
	}
}
