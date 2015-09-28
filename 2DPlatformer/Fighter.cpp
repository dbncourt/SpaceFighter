////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Fighter.h"

Fighter::Fighter() : GameObject()
{
	this->m_boxCollider = nullptr;
}

Fighter::Fighter(const Fighter& other)
{
}

Fighter::~Fighter()
{
}

bool Fighter::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	this->m_drawCollider = drawCollider;

	result = GameObject::Initialize(device, hwnd, screen, L"Fighter.dds", Bitmap::DimensionType{ 1152.0f, 216.0f }, Bitmap::DimensionType{ 144.0f, 108.0f }, 8, 7, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Fighter GameObject.", L"Error", MB_OK);
		return false;
	}

	GameObject::SetPosition(POINT{ 0.0f, (screen.height / 2) - 45.0f });
	
	int order[16] = { 7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 12, 13, 14, 15 };
	GameObject::SortFrameArray(order, 16);

	this->m_Collider = new BoxCollider();
	if (!this->m_Collider)
	{
		return false;
	}

	this->m_boxCollider = dynamic_cast<BoxCollider*>(this->m_Collider);
	result = this->m_boxCollider->Initialize(device, hwnd, screen, POINT{ 2, 25 }, BoxCollider::BoxDimensionsType{ 85, 35 });
	if (!result)
	{
		return false;
	}

	return true;
}

bool Fighter::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
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
			result = this->m_boxCollider->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}

	return true;
}

void Fighter::Frame(const InputHandler::ControlsType& controls)
{
	if (GameObject::GetActiveStatus())
	{
		GameObject::Frame(controls);

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
						GameObject::Move(D3DXVECTOR2(SHIP_SPEED * 2, 0));
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
	}

	this->m_boxCollider->Frame(GameObject::GetPosition());
}