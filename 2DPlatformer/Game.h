////////////////////////////////////////////////////////////////////////////////
// Filename: Game.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_H_
#define _GAME_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Fighter.h"
#include "FighterFlame.h"
#include "BulletManager.h"
#include "StarManager.h"
#include "MineManager.h"
#include "HealthManager.h"

class Game
{
public:
	Game();
	Game(const Game& other);
	~Game();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls);

private:
	void CheckCollisions();
	bool CheckCircleCircleCollision(CircleCollider* gO1Collider, CircleCollider* gO2Collider);
	bool CheckCircleRectangleCollision(BoxCollider* gO1Collider, CircleCollider* gO2Collider);

private:
	Fighter* m_Fighter;
	FighterFlame* m_FighterFlame;
	BulletManager* m_Bullets;
	StarManager* m_Stars;
	MineManager* m_Mines;
	HealthManager* m_HealthManager;
};
#endif