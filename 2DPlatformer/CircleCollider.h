////////////////////////////////////////////////////////////////////////////////
// Filename: CircleCollider.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CIRCLE_COLLIDER_H_ 
#define _CIRCLE_COLLIDER_H_ 

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Collider.h"
#include "ColorShader.h"

class CircleCollider : public Collider
{
public:
	CircleCollider();
	CircleCollider(const CircleCollider& other);
	~CircleCollider();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT offset, float radius);
	virtual void Shutdown() override;
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	const POINT GetCenter();
	const float GetRadius();

	void Frame(POINT center);

private:
	virtual bool InitializeBuffers(ID3D11Device* device) override;
	virtual bool UpdateBuffers(ID3D11DeviceContext* deviceContext, POINT center);

	const POINT GetCenterFromPosition(const POINT position);

private:
	POINT m_center;
	POINT m_offset;
	float m_radius;
	Bitmap::DimensionType m_screen;
	ColorShader* m_ColorShader;
};
#endif