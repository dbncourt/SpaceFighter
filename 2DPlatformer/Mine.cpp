////////////////////////////////////////////////////////////////////////////////
// Filename: Mine.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Mine.h"

Mine::Mine() : GameObject()
{
	this->m_CircleCollider = nullptr;
}

Mine::Mine(const Mine& other)
{
}

Mine::~Mine()
{
}

bool Mine::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	this->m_drawCollider = drawCollider;

	result = GameObject::Initialize(device, hwnd, screen, L"Mine.dds", Bitmap::DimensionType{ 16, 16 }, Bitmap::DimensionType{ 16, 16 }, 1, 0, true);;
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Mine GameObject.", L"Error", MB_OK);
		return false;
	}

	this->m_Collider = new CircleCollider();
	if (!this->m_Collider)
	{
		return false;
	}

	this->m_CircleCollider = dynamic_cast<CircleCollider*>(this->m_Collider);
	result = this->m_CircleCollider->Initialize(device, hwnd, screen, POINT{ 8, 8 }, 8.0f);
	if (!result)
	{
		return false;
	}

	return true;
}

void Mine::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetMovementDelayTime() > MOVEMENT_DELAY)
	{
		GameObject::Move();
		GameObject::ResetMovementDelayTime();
	}

	this->m_CircleCollider->Frame(GameObject::GetPosition());
}

bool Mine::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (GameObject::GetActiveStatus())
	{
		bool result;

		result = GameObject::Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}

		if (this->m_drawCollider)
		{
			result = this->m_CircleCollider->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}
