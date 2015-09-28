////////////////////////////////////////////////////////////////////////////////
// Filename: Exhaust.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EXHAUST_H_
#define _EXHAUST_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Exhaust : public GameObject
{
public:
	Exhaust();
	Exhaust(const Exhaust& other);
	~Exhaust();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
	virtual void Frame(const InputHandler::ControlsType& controls) override;
};
#endif