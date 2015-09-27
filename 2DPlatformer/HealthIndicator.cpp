////////////////////////////////////////////////////////////////////////////////
// Filename: HealthIndicator.cpp
////////////////////////////////////////////////////////////////////////////////
#include "HealthIndicator.h"

HealthIndicator::HealthIndicator() : GameObject()
{
	this->m_spriteName = nullptr;
}

HealthIndicator::HealthIndicator(const HealthIndicator& other)
{
}

HealthIndicator::~HealthIndicator()
{
}

bool HealthIndicator::Initialize(ID3D11Device* device, HWND hwnd, HealthIndicatorType healthIndicatorType, Bitmap::DimensionType screen, bool drawCollider)
{
	switch (healthIndicatorType)
	{
		case HealthIndicatorType::GREEN:
		{
			this->m_spriteName = L"HealthIndicator_Green.dds";
			break;
		}
		case HealthIndicatorType::YELLOW:
		{
			this->m_spriteName = L"HealthIndicator_Yellow.dds";
			break;
		}
		case HealthIndicatorType::RED:
		{
			this->m_spriteName = L"HealthIndicator_Red.dds";
			break;
		}
		default:
		{
			break;
		}
	}

	return HealthIndicator::Initialize(device, hwnd, screen, drawCollider);
}

bool HealthIndicator::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, this->m_spriteName, Bitmap::DimensionType{ 4, 13 }, Bitmap::DimensionType{ 4, 13 }, 1, 0, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the HealthIndicator GameObject", L"Error", MB_OK);
		return false;
	}

	return true;
}
