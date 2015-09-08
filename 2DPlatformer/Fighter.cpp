////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Fighter.h"

Fighter::Fighter()
{
	this->m_Sprite = nullptr;
	this->m_position = POINT{ 0, 0 };
	this->m_life = 100;
	this->m_lives = 3;
}

Fighter::Fighter(const Fighter& other)
{
}

Fighter::~Fighter()
{
}

bool Fighter::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* textureFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, POINT offset, int numberOfFramesAcross, int initialFrame, POINT initialPosition, int life, int lives)
{
	bool result;

	this->m_position = initialPosition;
	this->m_life = life;
	this->m_lives = lives;

	this->m_Sprite = new Sprite();
	if (!this->m_Sprite)
	{
		return false;
	}

	result = this->m_Sprite->Initialize(device, hwnd, screen, textureFileName, bitmap, sprite, offset, numberOfFramesAcross, initialFrame);
	if (!result)
	{
		return false;
	}

	int order[16] = { 7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15 };
	this->m_Sprite->SortFrameArray(order, 16);

	return true;
}

void Fighter::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Sprite);
}

bool Fighter::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	result = this->m_Sprite->Render(deviceContext, this->m_position, worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

void Fighter::InterpretAction(const InputHandler::ControlsType& controls)
{
	if (controls.up ^ controls.down)
	{
		if (controls.up)
		{
			if (this->m_position.y > 0)
			{
				this->m_position.y -= this->m_SPEED;
			}
			this->m_Sprite->IncrementFrame();
		}
		else if (controls.down)
		{
			if (this->m_position.y < (this->m_Sprite->GetBitmap()->GetScreenDimensions().height - this->m_Sprite->GetBitmap()->GetBitmapDimensions().height))
			{
				this->m_position.y += this->m_SPEED;
			}
			this->m_Sprite->DecrementFrame();
		}
	}
	else
	{
		if (this->m_Sprite->GetCurrentFrame() > (this->m_Sprite->GetAmountOfFrames() / 2))
		{
			this->m_Sprite->DecrementFrame();
		}
		if (this->m_Sprite->GetCurrentFrame() < (this->m_Sprite->GetAmountOfFrames() / 2))
		{
			this->m_Sprite->IncrementFrame();
		}
	}

	if (controls.right)
	{
		if (this->m_position.x < (this->m_Sprite->GetBitmap()->GetScreenDimensions().width - this->m_Sprite->GetBitmap()->GetBitmapDimensions().width))
		{
			this->m_position.x += this->m_SPEED;
		}
	}
	else if (controls.left)
	{
		if (this->m_position.x > 0)
		{
			this->m_position.x -= this->m_SPEED;
		}
	}

	if (controls.spaceBar)
	{
		//Shoot
	}
}