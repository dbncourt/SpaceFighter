////////////////////////////////////////////////////////////////////////////////
// Filename: ExplosionManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ExplosionManager.h"

ExplosionManager::ExplosionManager()
{
	this->m_Explosion = nullptr;
	this->m_device = nullptr;
	this->m_hwnd = nullptr;
}

ExplosionManager::ExplosionManager(const ExplosionManager& other)
{
}

ExplosionManager::~ExplosionManager()
{
}

bool ExplosionManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_ScreenDimensions = screen;

	this->m_Explosion = new Explosion();
	if (!this->m_Explosion)
	{
		return false;
	}

	result = this->m_Explosion->Initialize(device, hwnd, screen, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ExplosionManager GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ExplosionManager::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Explosion);

	for (GameObject* explosion : this->m_Explosions)
	{
		SAFE_SHUTDOWN(explosion);
	}
	this->m_Explosions.clear();
}

bool ExplosionManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_Explosion->GetActiveStatus())
	{
		bool result;

		for (GameObject* explosion : this->m_Explosions)
		{
			result = explosion->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}
	return true;
}

void ExplosionManager::Frame(const InputHandler::ControlsType& controls)
{
	if (this->m_Explosion->GetActiveStatus())
	{
		this->m_Explosion->Frame(controls);
		for (std::list<GameObject*>::iterator it = this->m_Explosions.begin(); it != this->m_Explosions.end();)
		{
			Explosion* explosion = dynamic_cast<Explosion*>(*it);
			explosion->Frame(controls);
			
			if (!explosion->IsAnimating())
			{
				it = this->m_Explosions.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

void ExplosionManager::AddExplosion(POINT position)
{
	if (this->m_Explosion->GetActiveStatus())
	{
		Explosion* explosion = new Explosion();
		explosion->Initialize(this->m_device, this->m_hwnd, this->m_ScreenDimensions, false);
		explosion->SetPosition(position);

		this->m_Explosions.push_back(explosion);
	}
}

void ExplosionManager::SetActiveStatus(bool status)
{
	this->m_Explosion->SetActiveStatus(status);
}

bool ExplosionManager::GetActiveStatus()
{
	return this->m_Explosion->GetActiveStatus();
}
