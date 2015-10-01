////////////////////////////////////////////////////////////////////////////////
// Filename: MineManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MineManager.h"

MineManager::MineManager()
{
	this->m_device = nullptr;
	this->m_hwnd = nullptr;
	this->m_ExplosionManager = nullptr;
	this->m_maxAmountOfMines = 0;
	this->m_activeStatus = true;
}

MineManager::MineManager(const MineManager& other)
{
}

MineManager::~MineManager()
{
}

bool MineManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfMines, bool drawColliders)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_screenDimensions = screen;
	this->m_maxAmountOfMines = maxAmountOfMines;
	this->m_drawCollider = drawColliders;

	for (int i = 0; i < maxAmountOfMines; i++)
	{
		MineManager::AddMine();
	}

	this->m_ExplosionManager = new ExplosionManager();
	if (!this->m_ExplosionManager)
	{
		return false;
	}

	result = this->m_ExplosionManager->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ExplosionManager GameObject.", L"Error", MB_OK);
		return false;
	}
	this->m_ExplosionManager->SetActiveStatus(true);

	return true;
}

void MineManager::Shutdown()
{
	for (GameObject* mine : this->m_Mines)
	{
		SAFE_SHUTDOWN(mine);
	}
	this->m_Mines.clear();

	SAFE_SHUTDOWN(this->m_ExplosionManager);
}

bool MineManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
	{
		bool result;

		for (GameObject* mine : this->m_Mines)
		{
			result = mine->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}

		result = this->m_ExplosionManager->Render(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}
	return true;
}

void MineManager::Frame(const InputHandler::ControlsType& controls)
{
	if (this->m_activeStatus)
	{
		for (GameObject* mine : this->m_Mines)
		{
			dynamic_cast<Mine*>(mine)->Frame(controls);
		}

		this->m_ExplosionManager->Frame(controls);

		MineManager::ValidateMinesBounds();

		bool result;
		POINT screenToInt{ static_cast<int>(this->m_screenDimensions.width), static_cast<int>(this->m_screenDimensions.height) };
		for (int i = 0; i < (this->m_maxAmountOfMines - this->m_Mines.size()); i++)
		{
			MineManager::AddMine();
		}
	}
}

void MineManager::ValidateMinesBounds()
{
	for (std::list<GameObject*>::iterator it = this->m_Mines.begin(); it != this->m_Mines.end();)
	{
		if ((*it)->GetPosition().x < 0)
		{
			it = this->m_Mines.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void MineManager::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool MineManager::GetActiveStatus()
{
	return this->m_activeStatus;
}

std::list<GameObject*>::iterator MineManager::GetListBegin()
{
	return this->m_Mines.begin();
}

std::list<GameObject*>::iterator MineManager::GetListEnd()
{
	return this->m_Mines.end();
}

std::list<GameObject*>::iterator MineManager::NotifyCollision(std::list<GameObject*>::iterator iterator)
{
	this->m_ExplosionManager->AddExplosion((*iterator)->GetPosition());
	return this->m_Mines.erase(iterator);
}

void MineManager::AddMine()
{
	Mine* mine = new Mine();
	mine->Initialize(this->m_device, this->m_hwnd, this->m_screenDimensions, this->m_drawCollider);
	
	mine->SetPosition(POINT{
		this->m_screenDimensions.width + 20.0f,
		rand() % (this->m_screenDimensions.height - 20)
	});

	mine->SetVelocity(D3DXVECTOR2(
		-(1 + rand() % 10),
		0
		));

	this->m_Mines.push_back(mine);
}
