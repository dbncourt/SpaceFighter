////////////////////////////////////////////////////////////////////////////////
// Filename: Mine.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MINE_H_
#define _MINE_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"
#include "CircleCollider.h"

class Mine : public GameObject
{
public:
	Mine();
	Mine(const Mine& other);
	~Mine();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) override;

	virtual void Frame(const InputHandler::ControlsType& controls) override;

private:
	CircleCollider* m_CircleCollider;
};
#endif