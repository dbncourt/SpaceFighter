////////////////////////////////////////////////////////////////////////////////
// Filename: HealthManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "HealthManager.h"

HealthManager::HealthManager()
{
	this->m_device = nullptr;
	this->m_hwnd = nullptr;
	this->m_HealthBar = nullptr;
	this->m_activeStatus = true;
}

HealthManager::HealthManager(const HealthManager& other)
{
}

HealthManager::~HealthManager()
{
}

bool HealthManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT position)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_screenDimensions = screen;

	this->m_HealthBar = new HealthBar();
	if (!this->m_HealthBar)
	{
		return false;
	}

	result = this->m_HealthBar->Initialize(device, hwnd, screen, false);
	if (!result)
	{
		return false;
	}
	this->m_HealthBar->SetPosition(position);

	result = HealthManager::ResetHealth();
	if (!result)
	{
		return false;
	}

	return true;
}

void HealthManager::Shutdown()
{
	SAFE_SHUTDOWN(this->m_HealthBar);

	for (GameObject* healthIndicator : this->m_HealthIndicators)
	{
		SAFE_SHUTDOWN(healthIndicator);
	}
	this->m_HealthIndicators.clear();
}

bool HealthManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
	{
		bool result;

		result = this->m_HealthBar->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}

		for (GameObject* healthIndicator : this->m_HealthIndicators)
		{
			result = healthIndicator->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

void HealthManager::DecrementHealth()
{
	if (this->m_activeStatus)
	{
		this->m_HealthIndicators.pop_back();
		if (this->m_HealthIndicators.empty())
		{
			HealthManager::ResetHealth();
		}
	}
}

bool HealthManager::ResetHealth()
{
	bool result;
	
	POINT relativePosition = POINT{
		this->m_HealthBar->GetPosition().x + 3,
		this->m_HealthBar->GetPosition().y + 2,
	};

	for (int i = 0; i < 20; i++)
	{
		HealthIndicator* healthIndicator = new HealthIndicator();
		if (!healthIndicator)
		{
			return false;
		}

		if (i > 9)
		{
			result = healthIndicator->Initialize(this->m_device, this->m_hwnd, HealthIndicator::HealthIndicatorType::GREEN, this->m_screenDimensions, false);
			if (!result)
			{
				return false;
			}
		}
		else if (i > 4)
		{
			result = healthIndicator->Initialize(this->m_device, this->m_hwnd, HealthIndicator::HealthIndicatorType::YELLOW, this->m_screenDimensions, false);
			if (!result)
			{
				return false;
			}
		}
		else
		{
			result = healthIndicator->Initialize(this->m_device, this->m_hwnd, HealthIndicator::HealthIndicatorType::RED, this->m_screenDimensions, false);
			if (!result)
			{
				return false;
			}
		}
		healthIndicator->SetPosition(POINT{
			relativePosition.x + (5 * i),
			relativePosition.y
		});
		this->m_HealthIndicators.push_back(healthIndicator);
	}

	return true;
}

void HealthManager::DecrementLife()
{

}

bool HealthManager::ResetLife()
{
	return true;
}

void HealthManager::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool HealthManager::GetActiveStatus()
{
	return this->m_activeStatus;
}
