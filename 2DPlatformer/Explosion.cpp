////////////////////////////////////////////////////////////////////////////////
// Filename: Explosion.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Explosion.h"


Explosion::Explosion() : GameObject()
{
}

Explosion::Explosion(const Explosion& other)
{
}

Explosion::~Explosion()
{
}

bool Explosion::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider)
{
	bool result;

	this->m_isAnimating = true;
	result = GameObject::Initialize(device, hwnd, screen, L"Explosion.dds", Bitmap::DimensionType{ 294, 72 }, Bitmap::DimensionType{ 42, 36 }, 7, -1, true);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Explosion GameObject.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Explosion::Frame(const InputHandler::ControlsType& controls)
{
	GameObject::Frame(controls);
	if (GameObject::GetAnimationDelayTime() > ANIMATION_DELAY)
	{
		if (GameObject::GetSprite()->GetCurrentFrame() != (GameObject::GetSprite()->GetAmountOfFrames() - 1))
		{
			GameObject::GetSprite()->IncrementFrame();
		}
		else
		{
 			GameObject::GetSprite()->ResetFrame();
			this->m_isAnimating = false;
		}
		GameObject::ResetAnimationDelayTime();
	}
}

bool Explosion::IsAnimating()
{
	return this->m_isAnimating;
}
