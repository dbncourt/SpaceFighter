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
	~Bullet();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;
};
#endif