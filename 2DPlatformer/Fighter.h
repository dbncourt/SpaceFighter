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
#include "Bullet.h"
#include "FighterFlame.h"

class Fighter : public GameObject
{
public:
	Fighter();
	Fighter(const Fighter& other);
	~Fighter();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen) override;
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	void GenerateTriBullet();
	void ValidateBulletsBounds();

private:
	int m_life;
	int m_lives;

	FighterFlame* m_FighterFlame;
	std::list<Bullet**> m_Bullets;

	const int SHIP_SPEED = 3;
	const float MOVEMENT_DELAY = 16.0f;
	const float ANIMATION_DELAY = 20.0f;
	const float SHOOT_DELAY = 30.0f;
};
#endif