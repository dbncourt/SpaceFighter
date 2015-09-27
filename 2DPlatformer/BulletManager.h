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

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

	std::list<GameObject*>::iterator GetListBegin();
	std::list<GameObject*>::iterator GetListEnd();
	std::list<GameObject*>::iterator NotifyCollision(std::list<GameObject*>::iterator iterator);

private:
	void GenerateTriBullet();
	void ValidateBulletsBounds();

private:
	ID3D11Device* m_device;
	HWND m_hwnd;

	Bullet* m_Bullet;
	std::list<GameObject*> m_Bullets;
	Bitmap::DimensionType m_screenDimensions;
	POINT m_relativePosition;

	const float SHOOT_DELAY = 100.0f;
	const bool DRAW_COLLIDER = false;
};
#endif