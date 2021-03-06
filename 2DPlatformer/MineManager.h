////////////////////////////////////////////////////////////////////////////////
// Filename: MineManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MINE_MANAGER_H_
#define _MINE_MANAGER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Mine.h"
#include "ExplosionManager.h"

class MineManager
{
public:
	MineManager();
	MineManager(const MineManager& other);
	~MineManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfMines, bool drawColliders);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls);

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

	std::list<GameObject*>::iterator GetListBegin();
	std::list<GameObject*>::iterator GetListEnd();
	std::list<GameObject*>::iterator NotifyCollision(std::list<GameObject*>::iterator iterator);

private:
	void ValidateMinesBounds();
	void AddMine();

private:
	ID3D11Device* m_device;
	HWND m_hwnd;

	std::list<GameObject*> m_Mines;
	ExplosionManager* m_ExplosionManager;
	Bitmap::DimensionType m_screenDimensions;

	int m_maxAmountOfMines;
	bool m_drawCollider;
	bool m_activeStatus;
};
#endif