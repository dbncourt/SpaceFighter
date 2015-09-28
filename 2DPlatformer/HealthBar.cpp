////////////////////////////////////////////////////////////////////////////////
// Filename: HealthBar.cpp
////////////////////////////////////////////////////////////////////////////////
#include "HealthBar.h"

HealthBar::HealthBar() : GameObject()
{
}

HealthBar::HealthBar(const HealthBar& other)
{
}

HealthBar::~HealthBar()
{
}

bool HealthBar::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"HealthBar.dds", Bitmap::DimensionType{ 105, 17 }, Bitmap::DimensionType{ 105, 17 }, 1, 0, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the HealthBar GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}
