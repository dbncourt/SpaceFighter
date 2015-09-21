////////////////////////////////////////////////////////////////////////////////
// Filename: MineManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "MineManager.h"

MineManager::MineManager()
{
	this->m_Mine = nullptr;
	this->m_ExplosionManager = nullptr;
}

MineManager::MineManager(const MineManager& other)
{
}

MineManager::~MineManager()
{
}

bool MineManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfMines)
{
	bool result;

	this->m_screenDimensions = screen;

	this->m_Mine = new Mine();
	if (!this->m_Mine)
	{
		return false;
	}

	result = this->m_Mine->Initialize(device, hwnd, screen, DRAW_COLLIDER);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the MineManager GameObject.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < maxAmountOfMines; i++)
	{
		Mine* mine = new Mine();
		if (!mine)
		{
			return false;
		}

		result = mine->Initialize(device, hwnd, screen, DRAW_COLLIDER);
		if (!result)
		{
			return false;
		}

		mine->SetPosition(POINT{
			(rand() % (screen.width / 2)) + screen.width / 2,
			rand() % (screen.height - 20)
		});

		mine->SetVelocity(D3DXVECTOR2(
			-(1 + rand() % 10),
			0
			));

		this->m_Mines.push_back(mine);
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
	SAFE_SHUTDOWN(this->m_Mine);

	for (GameObject* mine : this->m_Mines)
	{
		SAFE_SHUTDOWN(mine);
	}
	this->m_Mines.clear();

	SAFE_SHUTDOWN(this->m_ExplosionManager);
}

bool MineManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_Mine->GetActiveStatus())
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
	if (this->m_Mine->GetActiveStatus())
	{
		this->m_Mine->Frame(controls);

		for (GameObject* mine : this->m_Mines)
		{
			dynamic_cast<Mine*>(mine)->Frame(controls);
		}

		this->m_ExplosionManager->Frame(controls);

		MineManager::ValidateMinesBounds();
	}
}

void MineManager::ValidateMinesBounds()
{
	for (GameObject* mine : this->m_Mines)
	{
		if (mine->GetPosition().x < 0)
		{
			mine->SetPosition(POINT{
				this->m_screenDimensions.width + 10,
				rand() % this->m_screenDimensions.height
			});
		}
	}
}

std::list<GameObject*> MineManager::GetList()
{
	return this->m_Mines;
}

void MineManager::NotifyCollision(GameObject** mine)
{
	this->m_ExplosionManager->AddExplosion((*mine)->GetPosition());
	(*mine)->SetPosition(POINT{
		this->m_screenDimensions.width + 10,
		rand() % this->m_screenDimensions.height
	});
}

void MineManager::SetActiveStatus(bool status)
{
	this->m_Mine->SetActiveStatus(status);
}

bool MineManager::GetActiveStatus()
{
	return this->m_Mine->GetActiveStatus();
}