////////////////////////////////////////////////////////////////////////////////
// Filename: Bullet.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BULLET_H_
#define _BULLET_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"
#include "CircleCollider.h"

class Bullet : public GameObject
{
public:
	Bullet();
	Bullet(const Bullet& other);
	~Bullet();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	CircleCollider* m_CircleCollider;
};
#endif