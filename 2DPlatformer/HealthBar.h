////////////////////////////////////////////////////////////////////////////////
// Filename: HealthBar.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _HEALTH_BAR_H_
#define _HEALTH_BAR_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class HealthBar : public GameObject
{
public:
	HealthBar();
	HealthBar(const HealthBar& other);
	~HealthBar();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
};
#endif