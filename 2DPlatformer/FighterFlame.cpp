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

bool FighterFlame::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	result = GameObject::Initialize(device, hwnd, screen, L"Flame.dds", Bitmap::DimensionType{ 141, 26 }, Bitmap::DimensionType{ 47, 13 }, 3, -1, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the FighterFlame GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool FighterFlame::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	return GameObject::GetSprite()->Render(
		deviceContext,
		POINT{ GameObject::GetPosition().x - 26, GameObject::GetPosition().y + 47},
		wordMatrix, viewMatrix, projectionMatrix);
}


void FighterFlame::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);

	if (controls.right)
	{
		if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
		{
			GameObject::GetSprite()->IncrementFrame();
			GameObject::ResetAnimationDelayTime();
		}
	}
	else
	{
		GameObject::GetSprite()->ResetFrame();
	}
}