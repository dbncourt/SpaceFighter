////////////////////////////////////////////////////////////////////////////////
// Filename: ExhaustManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EXHAUST_MANAGER_H_
#define _EXHAUST_MANAGER_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Exhaust.h"

class ExhaustManager
{
public:
	ExhaustManager();
	ExhaustManager(const ExhaustManager& other);
	~ExhaustManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const InputHandler::ControlsType& controls, POINT position);

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

private:
	void ValidateExhaustsBounds();
	void AddExhaust(POINT position);

private:
	ID3D11Device* m_device;
	HWND m_hwnd;
	Bitmap::DimensionType m_screenDimensions;

	Exhaust* m_Exhaust;
	std::list<Exhaust*> m_Exhausts;

	const float SPAWN_DELAY = 16.0f;
};
#endif