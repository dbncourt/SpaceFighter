////////////////////////////////////////////////////////////////////////////////
// Filename: HealthManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _HEALTH_MANAGER_H
#define _HEALTH_MANAGER_H

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "HealthBar.h"
#include "HealthIndicator.h"
#include "LivesCount.h"
#include "Number.h"

class HealthManager
{
public:
	HealthManager();
	HealthManager(const HealthManager& other);
	~HealthManager();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT position);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void DecrementHealth();

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

private:
	bool ResetHealth();

	void DecrementLives();
	void ResetLives();

private:
	ID3D11Device* m_device;
	HWND m_hwnd;

	HealthBar* m_HealthBar;
	std::list<HealthIndicator*> m_HealthIndicators;
	Number* m_LivesNumber;
	LivesCount* m_LivesCount;

	Bitmap::DimensionType m_screenDimensions;

	bool m_activeStatus;
	int m_lives;
};
#endif