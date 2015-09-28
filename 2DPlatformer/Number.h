////////////////////////////////////////////////////////////////////////////////
// Filename: Number.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _NUMBER_H_
#define _NUMBER_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class Number : public GameObject
{
public:
	Number();
	Number(const Number& other);
	~Number();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
};

#endif