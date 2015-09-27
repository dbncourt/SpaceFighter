////////////////////////////////////////////////////////////////////////////////
// Filename: SquareCollider.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BOX_COLLIDER_H_ 
#define _BOX_COLLIDER_H_ 

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Collider.h"
#include "ColorShader.h"

class BoxCollider : public Collider
{
public:
	struct BoxDimensionsType
	{
		float base;
		float height;
	};

	struct BoxVerticesType
	{
		float x;
		float y;
		float width;
		float height;
	};

public:
	BoxCollider();
	BoxCollider(const BoxCollider& other);
	~BoxCollider();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT offset, BoxDimensionsType boxDimensions);
	virtual void Shutdown() override;
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Frame(const POINT position);

	const BoxDimensionsType GetBoxDimensions();
	const BoxVerticesType GetBoxColliderVertices();

private:
	virtual bool InitializeBuffers(ID3D11Device* device) override;
	virtual bool UpdateBuffers(ID3D11DeviceContext* deviceContext, const POINT position);

private:
	POINT m_position;
	POINT m_offset;
	BoxDimensionsType m_boxDimensions;
	ColorShader* m_ColorShader;
};
#endif