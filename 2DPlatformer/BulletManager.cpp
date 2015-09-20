////////////////////////////////////////////////////////////////////////////////
// Filename: BulletManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "BulletManager.h"

BulletManager::BulletManager()
{
	this->m_device = nullptr;
	this->m_hwnd = nullptr;
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
	this->m_ScreenDimensions = screen;

	this->m_Bullet = new Bullet();
	if (!this->m_Bullet)
	{
		return false;
	}
	
	result = this->m_Bullet->Initialize(device, hwnd, screen, DRAW_COLLIDER);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the BulletManager GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void BulletManager::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Bullet);

	for (GameObject* bullet : this->m_Bullets)
	{
		SAFE_SHUTDOWN(bullet);
	}
	this->m_Bullets.clear();
}

bool BulletManager::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
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

	return true;
}

void BulletManager::Frame(const InputHandler::ControlsType& controls)
{
	this->m_Bullet->Frame(controls);

	for (GameObject* bullet : this->m_Bullets)
	{
		dynamic_cast<Bullet*>(bullet)->Frame(controls);
	}

	if (controls.spaceBar)
	{
		if (this->m_Bullet->GetAnimationDelayTime() > SHOOT_DELAY)
		{
			BulletManager::GenerateTriBullet();
			this->m_Bullet->ResetAnimationDelayTime();
		}
	}

	BulletManager::ValidateBulletsBounds();
}

void BulletManager::GenerateTriBullet()
{
	Bullet* bullet;

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_ScreenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, 2));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_ScreenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, 0));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);

	bullet = new Bullet();
	bullet->Initialize(this->m_device, this->m_hwnd, this->m_ScreenDimensions, DRAW_COLLIDER);
	bullet->SetVelocity(D3DXVECTOR2(20, -2));
	bullet->SetPosition(POINT{ this->m_relativePosition.x + 138, this->m_relativePosition.y + 50 });
	this->m_Bullets.push_back(bullet);
}

void BulletManager::ValidateBulletsBounds()
{
	for (std::list<GameObject*>::iterator it = this->m_Bullets.begin(); it != this->m_Bullets.end();)
	{
		if ((*it)->GetPosition().x > this->m_ScreenDimensions.width)
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