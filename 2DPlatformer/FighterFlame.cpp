////////////////////////////////////////////////////////////////////////////////
// Filename: FighterFlame.cpp
////////////////////////////////////////////////////////////////////////////////
#include "FighterFlame.h"

FighterFlame::FighterFlame() : GameObject()
{
}

FighterFlame::FighterFlame(const FighterFlame& other)
{
}

FighterFlame::~FighterFlame()
{
}

bool FighterFlame::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Flame.dds", Bitmap::DimensionType{ 141, 26 }, Bitmap::DimensionType{ 47, 13 }, 3, -1, true);
	if (!result)
	{
		return false;
	}

	return true;
}

void FighterFlame::Frame(const InputHandler::ControlsType& controls)
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
					GameObject::Move(D3DXVECTOR2(0, -FLAME_SPEED));
				}
			}
			else if (controls.down)
			{
				if (GameObject::GetPosition().y < (GameObject::GetSprite()->GetBitmap()->GetScreenDimensions().height - GameObject::GetSprite()->GetBitmap()->GetBitmapDimensions().height))
				{
					GameObject::Move(D3DXVECTOR2(0, FLAME_SPEED));
				}
			}
		}
		if (controls.right ^ controls.left)
		{
			if (controls.right)
			{
				if (GameObject::GetPosition().x < (GameObject::GetSprite()->GetBitmap()->GetScreenDimensions().width - GameObject::GetSprite()->GetBitmap()->GetBitmapDimensions().width))
				{
					GameObject::Move(D3DXVECTOR2(FLAME_SPEED, 0));
					if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
					{
						GameObject::GetSprite()->IncrementFrame();
						GameObject::ResetAnimationDelayTime();
					}
				}
			}
			else if (controls.left)
			{
				if (GameObject::GetPosition().x > 0)
				{
					GameObject::Move(D3DXVECTOR2(-FLAME_SPEED, 0));
					GameObject::GetSprite()->ResetFrame();
				}
			}
		}
		else
		{
			GameObject::GetSprite()->ResetFrame();
		}
		GameObject::ResetMovementDelayTime();
	}
}
