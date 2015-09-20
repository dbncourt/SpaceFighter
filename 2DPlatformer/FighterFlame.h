////////////////////////////////////////////////////////////////////////////////
// Filename: FighterFlame.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIGHTER_FLAME_H_
#define _FIGHTER_FLAME_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class FighterFlame : public GameObject
{
public:
	FighterFlame();
	FighterFlame(const FighterFlame& other);
	~FighterFlame();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) override;
	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	const float ANIMATION_DELAY = 160.0f;
};

#endif