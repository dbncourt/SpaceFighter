////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Fighter.h"

Fighter::Fighter()
{
	this->m_Ship = nullptr;
	this->m_Flame = nullptr;
	this->m_position = POINT{ 0, 0 };
	this->m_life = 100;
	this->m_lives = 3;
	this->m_shipDelay = 0.0f;
	this->m_shootDelay = 0.0f;
	this->m_movementDelay = 0.0f;
	this->m_flameDelay = 0.0f;
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

	this->m_position = POINT{ 0, 0 };
	this->m_life = 100;
	this->m_lives = 3;

	this->m_Ship = new Sprite();
	if (!this->m_Ship)
	{
		return false;
	}

	result = this->m_Ship->Initialize(device, hwnd, screen, L"Fighter.dds", Bitmap::DimensionType{ 1152, 216 }, Bitmap::DimensionType{ 144, 108 }, POINT{ 0, 0 }, 8, 7);
	if (!result)
	{
		return false;
	}

	int order[16] = { 7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15 };
	this->m_Ship->SortFrameArray(order, 16);

	this->m_Flame = new Sprite();
	if (!this->m_Flame)
	{
		return false;
	}

	result = this->m_Flame->Initialize(device, hwnd, screen, L"Flame.dds", Bitmap::DimensionType{ 141, 26 }, Bitmap::DimensionType{ 47, 13 }, POINT{ 0, 0 }, 3, -1);
	if (!result)
	{
		return false;
	}

	this->m_Timer = new Timer();
	if (!this->m_Timer)
	{
		return false;
	}

	result = this->m_Timer->Initialize();
	if (!result)
	{
		return false;
	}

	return true;
}

void Fighter::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Ship);

	Fighter::ShutdownShootsList();
}

bool Fighter::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	result = this->m_Flame->Render(deviceContext, POINT{ this->m_position.x - 26, this->m_position.y + 47 }, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_Ship->Render(deviceContext, this->m_position, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

void Fighter::Frame(const InputHandler::ControlsType& controls)
{
	this->m_Timer->Frame();

	this->m_shipDelay += this->m_Timer->GetTime();
	this->m_shootDelay += this->m_Timer->GetTime();
	this->m_movementDelay += this->m_Timer->GetTime();
	this->m_flameDelay += this->m_Timer->GetTime();

	if (this->m_movementDelay > MOVEMENT_DELAY)
	{
		if (controls.up ^ controls.down)
		{
			if (controls.up)
			{
				if (this->m_position.y > 0)
				{
					this->m_position.y -= SHIP_SPEED;
				}

				if (this->m_shipDelay > SHIP_DELAY)
				{
					this->m_Ship->IncrementFrame();
					this->m_shipDelay = 0.0f;
				}
			}
			else if (controls.down)
			{
				if (this->m_position.y < (this->m_Ship->GetBitmap()->GetScreenDimensions().height - this->m_Ship->GetBitmap()->GetBitmapDimensions().height))
				{
					this->m_position.y += SHIP_SPEED;
				}
				if (this->m_shipDelay > SHIP_DELAY)
				{
					this->m_Ship->DecrementFrame();
					this->m_shipDelay = 0.0f;
				}
			}
		}
		else
		{
			if (this->m_Ship->GetCurrentFrame() > (this->m_Ship->GetAmountOfFrames() / 2))
			{
				if (this->m_shipDelay > SHIP_DELAY)
				{
					this->m_Ship->DecrementFrame();
					this->m_shipDelay = 0.0f;
				}
			}
			if (this->m_Ship->GetCurrentFrame() < (this->m_Ship->GetAmountOfFrames() / 2))
			{
				if (this->m_shipDelay > SHIP_DELAY)
				{
					this->m_Ship->IncrementFrame();
					this->m_shipDelay = 0.0f;
				}
			}
		}
		if (controls.right ^ controls.left)
		{
			if (controls.right)
			{
				if (this->m_position.x < (this->m_Ship->GetBitmap()->GetScreenDimensions().width - this->m_Ship->GetBitmap()->GetBitmapDimensions().width))
				{
					this->m_position.x += SHIP_SPEED;
				}
				if (this->m_flameDelay > FLAME_DELAY)
				{
					this->m_Flame->IncrementFrame();
					this->m_flameDelay = 0.0f;
				}
			}
			else if (controls.left)
			{
				if (this->m_position.x > 0)
				{
					this->m_position.x -= SHIP_SPEED;
				}
			}
		}
		else
		{
			this->m_Flame->ResetFrame();
		}
		this->m_movementDelay = 0.0f;
	}

	if (controls.spaceBar)
	{

	}
}

void Fighter::ShutdownShootsList()
{
	for (Sprite** s : this->m_Shoots)
	{
		SAFE_SHUTDOWN(*s);
	}

	this->m_Shoots.clear();
}

Sprite* Fighter::GenerateShoot(D3DXVECTOR2 direction)
{
	Sprite* shoot = new Sprite();
	if (!shoot)
	{
		return false;
	}
}