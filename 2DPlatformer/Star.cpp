////////////////////////////////////////////////////////////////////////////////
// Filename: Star.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Star.h"

Star::Star() : GameObject()
{
}

Star::Star(const Star& other)
{
}

Star::~Star()
{
}

bool Star::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Star.dds", Bitmap::DimensionType{ 1, 1 }, Bitmap::DimensionType{ 1, 1 }, 1, 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Star GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Star::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
		GameObject::ResetMovementDelayTime();
	}
}
