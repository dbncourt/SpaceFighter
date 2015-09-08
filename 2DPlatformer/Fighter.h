////////////////////////////////////////////////////////////////////////////////
// Filename: Fighter.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FIGHTER_H_
#define _FIGHTER_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Sprite.h"
#include "InputHandler.h"

class Fighter
{
public:
	Fighter();
	Fighter(const Fighter& other);
	~Fighter();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* textureFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, POINT offset, int numberOfFramesAcross, int initialFrame, POINT initialPosition, int life, int lives);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	void InterpretAction(const InputHandler::ControlsType& controls);

private:
	Sprite* m_Sprite;
	POINT m_position;
	int m_life;
	int m_lives;
	const int m_SPEED = 5;
};
#endif