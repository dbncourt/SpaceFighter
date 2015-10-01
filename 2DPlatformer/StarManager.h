////////////////////////////////////////////////////////////////////////////////
// Filename: StarManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STAR_MANAGER_H_
#define _STAR_MANAGER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Star.h"

class StarManager
{
public:
	StarManager();
	StarManager(const StarManager& other);
	~StarManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, int maxAmountOfStars);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls);

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

private:
	void ValidateStarsBounds();

private:
	std::list<Star*> m_Stars;
	Bitmap::DimensionType m_screenDimensions;

	bool m_activeStatus;
};
#endif