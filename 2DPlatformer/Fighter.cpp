////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Fighter.h"

Fighter::Fighter() : GameObject()
{
	this->m_life = 100;
	this->m_lives = 3;

	this->m_FighterFlame = nullptr;
}

Fighter::Fighter(const Fighter& other)
{
}

Fighter::~Fighter()
{
}

bool Fighter::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	this->m_life = 100;
	this->m_lives = 3;

	result = GameObject::Initialize(device, hwnd, screen, L"Fighter.dds", Bitmap::DimensionType{ 1152, 216 }, Bitmap::DimensionType{ 144, 108 }, 8, 7, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Fighter", L"Error", MB_OK);
		return false;
	}

	this->m_position = POINT{ 0, 0 };
	
	int order[16] = { 7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15 };
	GameObject::SortFrameArray(order, 16);

	this->m_FighterFlame = new FighterFlame();
	if (!this->m_FighterFlame)
	{
		return false;
	}

	result = this->m_FighterFlame->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize FighterFlame", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Fighter::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;
	
	result = GameObject::Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_FighterFlame->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	for (Bullet** bullet : this->m_Bullets)
	{
		if (bullet)
		{
			result = (*bullet)->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

void Fighter::Shutdown()
{
	GameObject::Shutdown();
	SAFE_SHUTDOWN(this->m_FighterFlame);
	
	for (Bullet** bullet : this->m_Bullets)
	{
		SAFE_SHUTDOWN(*bullet);
	}
	this->m_Bullets.clear();
}

void Fighter::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	this->m_FighterFlame->SetPosition(POINT{ this->m_position.x - 26, this->m_position.y + 47});
	this->m_FighterFlame->Frame(controls);

	for (Bullet** bullet : this->m_Bullets)
	{
		(*bullet)->Frame(controls);
	}

	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		if (controls.up ^ controls.down)
		{
			if (controls.up)
			{
				if (GameObject::GetPosition().y > 0)
				{
					GameObject::Move(D3DXVECTOR2(0, -SHIP_SPEED));
				}

				if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
				{
					GameObject::GetSprite()->IncrementFrame();
					GameObject::ResetAnimationDelayTime();
				}
			}
			else if (controls.down)
			{
				if (GameObject::GetPosition().y < (GameObject::GetSprite()->GetBitmap()->GetScreenDimensions().height - GameObject::GetSprite()->GetBitmap()->GetBitmapDimensions().height))
				{
					GameObject::Move(D3DXVECTOR2(0, SHIP_SPEED));
				}
				if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
				{
					GameObject::GetSprite()->DecrementFrame();
					GameObject::ResetAnimationDelayTime();
				}
			}
		}
		else
		{
			if (GameObject::GetSprite()->GetCurrentFrame() > (GameObject::GetSprite()->GetAmountOfFrames() / 2))
			{
				if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
				{
					GameObject::GetSprite()->DecrementFrame();
					GameObject::ResetAnimationDelayTime();
				}
			}
			if (GameObject::GetSprite()->GetCurrentFrame() < (GameObject::GetSprite()->GetAmountOfFrames() / 2))
			{
				if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
				{
					GameObject::GetSprite()->IncrementFrame();
					GameObject::ResetAnimationDelayTime();
				}
			}
		}
		if (controls.right ^ controls.left)
		{
			if (controls.right)
			{
				if (GameObject::GetPosition().x < (GameObject::GetSprite()->GetBitmap()->GetScreenDimensions().width - GameObject::GetSprite()->GetBitmap()->GetBitmapDimensions().width))
				{
					GameObject::Move(D3DXVECTOR2(SHIP_SPEED, 0));
				}
			}
			else if (controls.left)
			{
				if (GameObject::GetPosition().x > 0)
				{
					GameObject::Move(D3DXVECTOR2(-SHIP_SPEED, 0));
				}
			}
		}
		GameObject::ResetMovementDelayTime();
	}

	if (controls.spaceBar)
	{
		Fighter::GenerateTriBullet();
	}
	Fighter::ValidateBulletsBounds();
}

void Fighter::GenerateTriBullet()
{
	Bullet* upBullet = new Bullet();
	upBullet->Initialize(this->m_FighterFlame->GetDevice(), this->m_FighterFlame->GetHWND(), this->m_FighterFlame->GetSprite()->GetBitmap()->GetScreenDimensions());
	upBullet->SetVelocity(D3DXVECTOR2(20, 2));
	upBullet->SetPosition(GameObject::GetPosition());
	upBullet->Move();
	this->m_Bullets.push_back(&upBullet);

	Bullet* middleBullet = new Bullet();
	middleBullet->Initialize(this->m_FighterFlame->GetDevice(), this->m_FighterFlame->GetHWND(), this->m_FighterFlame->GetSprite()->GetBitmap()->GetScreenDimensions());
	middleBullet->SetVelocity(D3DXVECTOR2(20, 0));
	middleBullet->SetPosition(GameObject::GetPosition());
	middleBullet->Move();
	this->m_Bullets.push_back(&middleBullet);

	Bullet* downBullet = new Bullet();
	downBullet->Initialize(this->m_FighterFlame->GetDevice(), this->m_FighterFlame->GetHWND(), this->m_FighterFlame->GetSprite()->GetBitmap()->GetScreenDimensions());
	downBullet->SetVelocity(D3DXVECTOR2(20, -2));
	downBullet->SetPosition(GameObject::GetPosition());
	downBullet->Move();
	this->m_Bullets.push_back(&downBullet);
}

void Fighter::ValidateBulletsBounds()
{
	for (std::list<Bullet**>::iterator it = this->m_Bullets.begin(); it != this->m_Bullets.end(); it++)
	{
		if ((*(*(&(it)._Ptr->_Myval)))->GetPosition().x > GameObject::GetSprite()->GetBitmap()->GetScreenDimensions().width)
		{
			SAFE_SHUTDOWN(**it);
			this->m_Bullets.erase(it);
		}
	}
}