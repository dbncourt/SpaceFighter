////////////////////////////////////////////////////////////////////////////////
// Filename: Bullet.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLET_H_
#define _BULLET_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	Bullet(const Bullet& other);
	~Bullet();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	const float MOVEMENT_DELAY = 16.0f;
};
#endif