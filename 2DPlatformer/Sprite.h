////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPRITE_H_ 
#define _SPRITE_H_ 

/////////////
// MACROS  //
/////////////
#define SAFE_ARRAY_DELETE(p) { if(p) { delete[](p); (p) = nullptr;}}

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Bitmap.h"
#include "TextureShader.h"

class Sprite
{
public:
	struct FrameType
	{
		float x;
		float y;
		float width;
		float height;
	};

public:
	Sprite();
	Sprite(const Sprite& other);
	~Sprite();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* textureFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, POINT offset, int numberOfFramesAcross, int initialFrame);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, POINT position, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix);

	void IncrementFrame();
	void DecrementFrame();
	void ResetFrame();

	int GetCurrentFrame();
	int GetAmountOfFrames();

	void SortFrameArray(const int* framesOrder, int size);

	Bitmap* GetBitmap();

private:
	void InitializeFrameArray(Bitmap::DimensionType bitmap, Bitmap::DimensionType dimensions, POINT offset, int numberOfFrameAcross);

private:
	FrameType* m_frameArray;
	int m_currentFrame;
	int m_initialFrame;
	int m_amountOfFrames;
	Bitmap* m_Bitmap;
	TextureShader* m_TextureShader;
};

#endif