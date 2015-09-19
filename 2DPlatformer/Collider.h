////////////////////////////////////////////////////////////////////////////////
// Filename: Collider.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx10math.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Bitmap.h"

class Collider
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXCOLOR color;
	};

public:
	Collider();
	Collider(const Collider& other);
	~Collider();

	virtual bool Initialize(ID3D11Device* device, Bitmap::DimensionType screen);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

protected:
	virtual bool InitializeBuffers(ID3D11Device* device) = 0;
	virtual void ShutdownBuffers();
	virtual void RenderBuffers(ID3D11DeviceContext* deviceContext);

protected:
	ID3D11Buffer* m_vertexBuffer;
	UINT m_vertexCount;
	ID3D11Buffer* m_indexBuffer;
	UINT m_indexCount;
	Bitmap::DimensionType m_screen;
};
#endif