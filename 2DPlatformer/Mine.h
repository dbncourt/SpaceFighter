////////////////////////////////////////////////////////////////////////////////
// Filename: Mine.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MINE_H_
#define _MINE_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Mine : public GameObject
{
public:
	Mine();
	Mine(const Mine& other);
	~Mine();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	virtual void Frame(const InputHandler::ControlsType& controls) override;
};
#endif