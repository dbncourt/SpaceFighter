////////////////////////////////////////////////////////////////////////////////
// Filename: StarManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "StarManager.h"

StarManager::StarManager()
{
	this->m_Star = nullptr;
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

	this->m_Star = new Star();
	if (!this->m_Star)
	{
		return false;
	}

	result = this->m_Star->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the StarManager GameObject.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < maxAmountOfStars; i++)
	{
		Star* star = new Star();
		if (!star)
		{
			return false;
		}

		result = star->Initialize(device, hwnd, screen);
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
	SAFE_SHUTDOWN(this->m_Star);

	for (Star* star : this->m_Stars)
	{
		SAFE_SHUTDOWN(star);
	}

	this->m_Stars.clear();
}

bool StarManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
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

	return true;
}

void StarManager::Frame(const InputHandler::ControlsType& controls)
{
	this->m_Star->Frame(controls);

	for (Star* star : this->m_Stars)
	{
		star->Frame(controls);
	}

	StarManager::ValidateStarsBounds();
}

void StarManager::ValidateStarsBounds()
{
	for (std::list<Star*>::iterator it = this->m_Stars.begin(); it != this->m_Stars.end(); it++)
	{
		if ((*it)->GetPosition().x < 0)
		{
			(*it)->SetPosition(
				POINT{
				this->m_screenDimensions.width,
				(*it)->GetPosition().y 
			});
		}
	}
}
