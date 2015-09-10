////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#define SHIP_SPEED 5
#define MOVEMENT_DELAY 16.0f
#define SHIP_DELAY 35.0f
#define FLAME_DELAY 50.0f
#define SHOOT_DELAY 35.0f

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Sprite.h"
#include "InputHandler.h"
#include "Timer.h"

class Fighter
{
public:
	Fighter();
	Fighter(const Fighter& other);
	~Fighter();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	void Frame(const InputHandler::ControlsType& controls);

private:
	Sprite* GenerateShoot(D3DXVECTOR2 direction);
	void ShutdownShootsList();

private:
	Sprite* m_Ship;
	Sprite* m_Flame;
	std::list<Sprite**> m_Shoots;
	Timer* m_Timer;
	POINT m_position;
	int m_life;
	int m_lives;
	float m_movementDelay;
	float m_shipDelay;
	float m_flameDelay;
	float m_shootDelay;
};
#endif