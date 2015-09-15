////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.h
////////////////////////////////////////////////////////////////////////////////
#include "GameObject.h"

GameObject::GameObject()
{
	this->m_Sprite = nullptr;
	this->m_Timer = nullptr;
	this->m_movementDelay = 0.0f;
	this->m_animationDelay = 0.0f;
}

GameObject::GameObject(const GameObject& other)
{
}

GameObject::~GameObject()
{
}

bool GameObject::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* spriteFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, int numberOfFramesAcross, int initialFrame, bool useTimer)
{
	bool result;

	this->m_device = device;
	this->m_hwnd = hwnd;

	this->m_Sprite = new Sprite();
	if (!this->m_Sprite)
	{
		return false;
	}

	result = this->m_Sprite->Initialize(device, hwnd, screen, spriteFileName, bitmap, sprite, numberOfFramesAcross, initialFrame);
	if (!result)
	{
		return false;
	}

	if (useTimer)
	{
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
	}
	return true;
}

void GameObject::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Sprite);
	SAFE_DELETE(this->m_Timer);
}

bool GameObject::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	return this->m_Sprite->Render(deviceContext, this->m_position, wordMatrix, viewMatrix, projectionMatrix);
}

void GameObject::Move()
{
	this->m_position.x += this->m_velocity.x;
	this->m_position.y += this->m_velocity.y;
}

void GameObject::Move(const D3DXVECTOR2 vector)
{
	this->m_position.x += vector.x;
	this->m_position.y += vector.y;
}

void GameObject::Frame(const InputHandler::ControlsType& controls)
{
	if (this->m_Timer)
	{
		this->m_Timer->Frame();

		this->m_movementDelay += this->m_Timer->GetTime();
		this->m_animationDelay += this->m_Timer->GetTime();
	}
}

void GameObject::SortFrameArray(const int* framesOrder, int size)
{
	this->m_Sprite->SortFrameArray(framesOrder, size);
}

void GameObject::SetPosition(const POINT position)
{
	this->m_position = position;
}

const POINT GameObject::GetPosition()
{
	return this->m_position;
}

const POINT* GameObject::GetPositionAddress()
{
	return &this->m_position;
}

void GameObject::SetVelocity(const D3DXVECTOR2& velocity)
{
	this->m_velocity = velocity;
}

const D3DXVECTOR2 GameObject::GetVelocity()
{
	return this->m_velocity;
}

void GameObject::SetStatus(const bool status)
{
	this->m_active = status;
}

bool GameObject::GetStatus()
{
	return this->m_active;
}

Sprite* GameObject::GetSprite()
{
	return this->m_Sprite;
}

float GameObject::GetAnimationDelayTime()
{
	return this->m_animationDelay;
}

void GameObject::ResetMovementDelayTime()
{
	this->m_movementDelay = 0.0f;
}

float GameObject::GetMovementDelayTime()
{
	return this->m_animationDelay;
}

void GameObject::ResetAnimationDelayTime()
{
	this->m_animationDelay = 0.0f;
}

ID3D11Device* GameObject::GetDevice()
{
	return this->m_device;
}

HWND GameObject::GetHWND()
{
	return this->m_hwnd;
}