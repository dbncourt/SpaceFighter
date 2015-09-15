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

class MineManager
{
public:
	MineManager();
	MineManager(const MineManager& other);
	~MineManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfMines);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls);

private:
	void ValidateMinesBounds();

private:
	Mine* m_Mine;
	std::list<Mine*> m_Mines;
	Bitmap::DimensionType m_screenDimensions;
};
#endif