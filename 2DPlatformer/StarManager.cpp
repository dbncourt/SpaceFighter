////////////////////////////////////////////////////////////////////////////////
// Filename: StarManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "StarManager.h"

StarManager::StarManager()
{
	this->m_activeStatus = true;
}

StarManager::StarManager(const StarManager& other)
{
}

StarManager::~StarManager()
{
}

bool StarManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfStars)
{
	bool result;

	this->m_screenDimensions = screen;

	for (int i = 0; i < maxAmountOfStars; i++)
	{
		Star* star = new Star();
		if (!star)
		{
			return false;
		}

		result = star->Initialize(device, hwnd, screen, false);
		if (!result)
		{
			return false;
		}

		star->SetPosition(
			POINT{ 
			rand() % screen.width, 
			rand() % screen.height 
		});

		star->SetVelocity(
			D3DXVECTOR2(
			-(1 + rand() % 16)
			,0
			));

		this->m_Stars.push_back(star);
	}

	return true;
}

void StarManager::Shutdown()
{
	for (Star* star : this->m_Stars)
	{
		SAFE_SHUTDOWN(star);
	}

	this->m_Stars.clear();
}

bool StarManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
	{
		bool result;

		for (Star* star : this->m_Stars)
		{
			result = star->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}
	return true;
}

void StarManager::Frame(const InputHandler::ControlsType& controls)
{
	if (this->m_activeStatus)
	{
		for (Star* star : this->m_Stars)
		{
			star->Frame(controls);
		}

		StarManager::ValidateStarsBounds();
	}
}

void StarManager::ValidateStarsBounds()
{
	for (Star* star : this->m_Stars)
	{
		if (star->GetPosition().x < 0)
		{
			star->SetPosition(
				POINT{
				this->m_screenDimensions.width,
				star->GetPosition().y
			});
		}
	}
}

void StarManager::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool StarManager::GetActiveStatus()
{
	return this->m_activeStatus;
}
