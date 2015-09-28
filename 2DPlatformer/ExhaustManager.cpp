////////////////////////////////////////////////////////////////////////////////
// Filename: ExhaustManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ExhaustManager.h"

ExhaustManager::ExhaustManager()
{
	this->m_device = nullptr;
	this->m_hwnd = nullptr;

	this->m_Exhaust = nullptr;
}

ExhaustManager::ExhaustManager(const ExhaustManager& other)
{
}

ExhaustManager::~ExhaustManager()
{
}

bool ExhaustManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_screenDimensions = screen;

	this->m_Exhaust = new Exhaust();
	if (!this->m_Exhaust)
	{
		return false;
	}

	result = this->m_Exhaust->Initialize(device, hwnd, screen, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ExhaustManager GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ExhaustManager::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Exhaust);

	for (GameObject* exhaust : this->m_Exhausts)
	{
		SAFE_SHUTDOWN(exhaust);
	}
	this->m_Exhausts.clear();
}

bool ExhaustManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_Exhaust->GetActiveStatus())
	{
		bool result;

		for (GameObject* exhaust : this->m_Exhausts)
		{
			result = exhaust->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}
	return true;
}

void ExhaustManager::Frame(const InputHandler::ControlsType& controls, POINT position)
{
	if (this->m_Exhaust->GetActiveStatus())
	{
		this->m_Exhaust->Frame(controls);
		for (Exhaust* exhaust : this->m_Exhausts)
		{
			exhaust->Frame(controls);
		}

		ExhaustManager::ValidateExhaustsBounds();
		if (controls.right)
		{
			if (this->m_Exhaust->GetAnimationDelayTime() > SPAWN_DELAY)
			{
				ExhaustManager::AddExhaust(position);
				this->m_Exhaust->ResetAnimationDelayTime();
			}
		}
	}
}

void ExhaustManager::SetActiveStatus(bool status)
{
	this->m_Exhaust->SetActiveStatus(status);
}

bool ExhaustManager::GetActiveStatus()
{
	return this->m_Exhaust->GetActiveStatus();
}

void ExhaustManager::ValidateExhaustsBounds()
{
	for (std::list<Exhaust*>::iterator it = this->m_Exhausts.begin(); it != this->m_Exhausts.end();)
	{
		if ((*it)->GetPosition().x < 0)
		{
			it = this->m_Exhausts.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ExhaustManager::AddExhaust(POINT position)
{
	Exhaust* exhaust = new Exhaust();
	exhaust->Initialize(this->m_device, this->m_hwnd, this->m_screenDimensions, false);
	exhaust->SetPosition(
		POINT{
		position.x,
		rand() % 7 + position.y
	});

	exhaust->SetVelocity(
		D3DXVECTOR2(
		-(4 + rand() % 10)
		, 0
		));

	this->m_Exhausts.push_back(exhaust);
}
