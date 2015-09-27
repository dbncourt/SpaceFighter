////////////////////////////////////////////////////////////////////////////////
// Filename: HealthIndicator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _HEALTH_INDICATOR_H_
#define _HEALTH_INDICATOR_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"

class HealthIndicator : public GameObject
{
public:
	enum HealthIndicatorType
	{
		RED,
		YELLOW,
		GREEN
	};

public:
	HealthIndicator();
	HealthIndicator(const HealthIndicator& other);
	~HealthIndicator();
	bool Initialize(ID3D11Device* device, HWND hwnd, HealthIndicatorType healthIndicatorType, Bitmap::DimensionType screen, bool drawCollider);

private:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;

private:
	WCHAR* m_spriteName;
};
#endif