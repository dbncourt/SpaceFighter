////////////////////////////////////////////////////////////////////////////////
// Filename: BulletManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Bullet.h"

class BulletManager
{
public:
	BulletManager();
	BulletManager(const BulletManager& other);
	~BulletManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void SetRelativePosition(POINT relativePosition);

	void Frame(const InputHandler::ControlsType& controls);

	std::list<GameObject*> GetList();

private:
	void GenerateTriBullet();
	void ValidateBulletsBounds();

private:
	ID3D11Device* m_device;
	HWND m_hwnd;
	
	Bullet* m_Bullet;
	std::list<GameObject*> m_Bullets;
	Bitmap::DimensionType m_ScreenDimensions;
	POINT m_relativePosition;

	const float SHOOT_DELAY = 80.0f;
};
#endif