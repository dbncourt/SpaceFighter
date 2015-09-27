////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIGHTER_H_
#define _FIGHTER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"
#include "BoxCollider.h"

class Fighter : public GameObject
{
public:
	Fighter();
	Fighter(const Fighter& other);
	~Fighter();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	int m_life;
	int m_lives;
	BoxCollider* m_boxCollider;

	const int SHIP_SPEED = 3;
	const float ANIMATION_DELAY = 20.0f;
};
#endif