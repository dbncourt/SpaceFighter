////////////////////////////////////////////////////////////////////////////////
// Filename: Star.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STAR_H_
#define _STAR_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Star : public GameObject
{
public:
	Star();
	Star(const Star& other);
	~Star();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	virtual void Frame(const InputHandler::ControlsType& controls) override;
};
#endif