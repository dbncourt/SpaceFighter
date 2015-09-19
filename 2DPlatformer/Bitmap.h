////////////////////////////////////////////////////////////////////////////////
// Filename: Bitmap.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAP_H_ 
#define _BITMAP_H_ 

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx10math.h>

/////////////
// MACROS  //
/////////////
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr;}}
#define SAFE_SHUTDOWN(p) { if(p) { (p)->Shutdown(); delete (p); (p) = nullptr;}}

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"

class Bitmap
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct DimensionType
	{
		int width;
		int height;
	};

public:
	Bitmap();
	Bitmap(const Bitmap& other);
	~Bitmap();

	bool Initialize(ID3D11Device* device, DimensionType screen, WCHAR* textureFileName, DimensionType bitmap);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, POINT position, D3DXVECTOR4 textureCoordinates);

	UINT GetIndexCount();
	void GetIndexCount(_Out_ UINT& indexCount);

	ID3D11ShaderResourceView* GetTexture();
	void GetTexture(_Out_ ID3D11ShaderResourceView*& texture);

	DimensionType GetScreenDimensions();
	DimensionType GetBitmapDimensions();

	int GetVertexCount();
	int GetInstanceCount();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext, POINT position, D3DXVECTOR4 textureCoordinates);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* textureFileName);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer;
	UINT m_vertexCount;
	ID3D11Buffer* m_indexBuffer;
	UINT m_indexCount;
	Texture* m_Texture;
	DimensionType m_screen;
	DimensionType m_bitmap;
};

#endif