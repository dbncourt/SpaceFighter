////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#define SHIP_SPEED 5
#define FRAME_TRANSITION_DELAY 35.0f
#define SHOOT_DELAY 35.0f

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

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* textureFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, POINT offset, int numberOfFramesAcross, int initialFrame, POINT initialPosition, int life, int lives);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	void Frame(const InputHandler::ControlsType& controls);

private:
	Sprite* m_Sprite;
	Timer* m_Timer;
	POINT m_position;
	int m_life;
	int m_lives;
	float m_transitionDelay;
	float m_shootDelay;
};
#endif