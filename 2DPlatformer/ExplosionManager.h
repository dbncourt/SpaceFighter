////////////////////////////////////////////////////////////////////////////////
// Filename: ExplosionManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EXPLOSION_MANAGER_H_
#define _EXPLOSION_MANAGER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Explosion.h"

class ExplosionManager
{
public:
	ExplosionManager();
	ExplosionManager(const ExplosionManager& other);
	~ExplosionManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls);

	void AddExplosion(POINT position);

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

private:
	ID3D11Device* m_device;
	HWND m_hwnd;

	std::list<GameObject*> m_Explosions;
	Bitmap::DimensionType m_ScreenDimensions;

	bool m_activeStatus;
};
#endif