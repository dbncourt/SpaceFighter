////////////////////////////////////////////////////////////////////////////////
// Filename: BulletManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "BulletManager.h"

BulletManager::BulletManager()
{
	this->m_Timer = nullptr;
	this->m_device = nullptr;
	this->m_hwnd = nullptr;
	this->m_activeStatus = true;
	this->m_accumulatedTime = 0.0f;
}

BulletManager::BulletManager(const BulletManager& other)
{
}

BulletManager::~BulletManager()
{
}

bool BulletManager::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;
	this->m_screenDimensions = screen;

	this->m_Timer = new Timer();
	if (!this->m_Timer)
	{
		return false;
	}
	
	result = this->m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the BulletManager Timer.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void BulletManager::Shutdown()
{
	SAFE_DELETE(this->m_Timer);

	for (GameObject* bullet : this->m_Bullets)
	{
		SAFE_SHUTDOWN(bullet);
	}
	this->m_Bullets.clear();
}

bool BulletManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
	{
		bool result;

		for (GameObject* bullet : this->m_Bullets)
		{
			result = bullet->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

void BulletManager::Frame(const InputHandler::ControlsType& controls)
{
	if (this->m_activeStatus)
	{
		this->m_Timer->Frame();
		for (GameObject* bullet : this->m_Bullets)
		{
			dynamic_cast<Bullet*>(bullet)->Frame(controls);
		}

		this->m_accumulatedTime += this->m_Timer->GetTime();
		if (controls.spaceBar)
		{
			if (this->m_accumulatedTime > SHOOT_DELAY)
			{
				BulletManager::GenerateTriBullet();
				this->m_accumulatedTime = 0.0f;
			}
		}

		BulletManager::ValidateBulletsBounds();
	}
}

void BulletManager::GenerateTriBullet()
{
	Bullet* bullet;

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_screenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, 2));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_screenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, 0));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_screenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, -2));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);
}

void BulletManager::ValidateBulletsBounds()
{
	for (std::list<GameObject*>::iterator it = this->m_Bullets.begin(); it != this->m_Bullets.end();)
	{
		if ((*it)->GetPosition().x > this->m_screenDimensions.width)
		{
			it = this->m_Bullets.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void BulletManager::SetRelativePosition(POINT relativePosition)
{
	this->m_relativePosition = relativePosition;
}

std::list<GameObject*>::iterator BulletManager::GetListBegin()
{
	return this->m_Bullets.begin();
}

std::list<GameObject*>::iterator BulletManager::GetListEnd()
{
	return this->m_Bullets.end();
}

std::list<GameObject*>::iterator BulletManager::NotifyCollision(std::list<GameObject*>::iterator iterator)
{
	return this->m_Bullets.erase(iterator);
}

void BulletManager::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool BulletManager::GetActiveStatus()
{
	return this->m_activeStatus;
}