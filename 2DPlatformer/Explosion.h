////////////////////////////////////////////////////////////////////////////////
// Filename: Explosion.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Explosion : public GameObject
{
public:
	Explosion();
	Explosion(const Explosion& other);
	~Explosion();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

	bool IsAnimating();

private:
	bool m_isAnimating;
	const float ANIMATION_DELAY = 32.0f;
};
#endif