////////////////////////////////////////////////////////////////////////////////
// Filename: Mine.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Mine.h"

Mine::Mine() : GameObject()
{
}

Mine::Mine(const Mine& other)
{
}

Mine::~Mine()
{
}

bool Mine::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Mine.dds", Bitmap::DimensionType{ 16, 16 }, Bitmap::DimensionType{ 16, 16 }, 1, 0, true);;
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Mine GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Mine::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
	}
}
