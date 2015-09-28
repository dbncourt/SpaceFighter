////////////////////////////////////////////////////////////////////////////////
// Filename: LivesCount.cpp
////////////////////////////////////////////////////////////////////////////////
#include "LivesCount.h"

LivesCount::LivesCount() : GameObject()
{
}

LivesCount::LivesCount(const LivesCount& other)
{
}

LivesCount::~LivesCount()
{
}

bool LivesCount::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"LifeCounter.dds", Bitmap::DimensionType{ 67, 38 }, Bitmap::DimensionType{ 67, 38 }, 1, 0, false);
	if (!result)
	{
		return false;
	}

	return true;
}