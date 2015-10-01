////////////////////////////////////////////////////////////////////////////////
// Filename: ExplosionManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ExplosionManager.h"

ExplosionManager::ExplosionManager()
{
	this->m_device = nullptr;
	this->m_hwnd = nullptr;

	this->m_activeStatus = true;
}

ExplosionManager::ExplosionManager(const ExplosionManager& other)
{
}

ExplosionManager::~ExplosionManager()
{
}

bool ExplosionManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_ScreenDimensions = screen;

	return true;
}

void ExplosionManager::Shutdown()
{
	for (GameObject* explosion : this->m_Explosions)
	{
		SAFE_SHUTDOWN(explosion);
	}
	this->m_Explosions.clear();
}

bool ExplosionManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
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
	if (this->m_activeStatus)
	{
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
	if (this->m_activeStatus)
	{
		Explosion* explosion = new Explosion();
		explosion->Initialize(this->m_device, this->m_hwnd, this->m_ScreenDimensions, false);
		explosion->SetPosition(position);

		this->m_Explosions.push_back(explosion);
	}
}

void ExplosionManager::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool ExplosionManager::GetActiveStatus()
{
	return this->m_activeStatus;
}
