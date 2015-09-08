////////////////////////////////////////////////////////////////////////////////
// Filename: Sprite.h
////////////////////////////////////////////////////////////////////////////////
#include "Sprite.h"

Sprite::Sprite()
{
	this->m_frameArray = nullptr;
	this->m_Bitmap = nullptr;
	this->m_TextureShader = nullptr;
}

Sprite::Sprite(const Sprite& other)
{
}

Sprite::~Sprite()
{
}

bool Sprite::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* textureFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, POINT offset, int numberOfFramesAcross, int initialFrame)
{
	bool result;

	this->m_currentFrame = initialFrame;

	this->m_Bitmap = new Bitmap();
	if (!this->m_Bitmap)
	{
		return false;
	}

	result = this->m_Bitmap->Initialize(device, screen, textureFileName, sprite);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Bitmap Object.", L"Error", MB_OK);
		return false;
	}

	this->m_TextureShader = new TextureShader();
	if (!this->m_TextureShader)
	{
		return false;
	}

	result = this->m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextureShader Object.", L"Error", MB_OK);
		return false;
	}

	Sprite::InitializeFrameArray(bitmap, sprite, offset, numberOfFramesAcross);

	return true;
}

void Sprite::Shutdown()
{
	SAFE_ARRAY_DELETE(this->m_frameArray);
	SAFE_SHUTDOWN(this->m_Bitmap);
	SAFE_SHUTDOWN(this->m_TextureShader);
}

bool Sprite::Render(ID3D11DeviceContext* deviceContext, POINT position, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	D3DXVECTOR4 frameCoordinates = D3DXVECTOR4(this->m_frameArray[this->m_currentFrame].x, this->m_frameArray[this->m_currentFrame].y, this->m_frameArray[this->m_currentFrame].width, this->m_frameArray[this->m_currentFrame].height);
	result = this->m_Bitmap->Render(deviceContext, position, frameCoordinates);
	if (!result)
	{
		return false;
	}

	result = this->m_TextureShader->Render(deviceContext, this->m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, this->m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	return true;
}

void Sprite::IncrementFrame()
{
	if (this->m_currentFrame < (this->m_amountOfFrames - 1))
	{
		++this->m_currentFrame;
	}
}

void Sprite::DecrementFrame()
{
	if (this->m_currentFrame > 0)
	{
		--this->m_currentFrame;
	}
}

int Sprite::GetCurrentFrame()
{
	return this->m_currentFrame;
}

int Sprite::GetAmountOfFrames()
{
	return this->m_amountOfFrames;
}

void Sprite::InitializeFrameArray(Bitmap::DimensionType bitmap, Bitmap::DimensionType dimensions, POINT offset, int numberOfFrameAcross)
{
	int verticalAmountOfFrames = (bitmap.height / dimensions.height);
	this->m_amountOfFrames = numberOfFrameAcross * verticalAmountOfFrames;
	this->m_frameArray = new FrameType[this->m_amountOfFrames];

	for (int i = 0; i < verticalAmountOfFrames; i++)
	{
		for (int j = 0; j < numberOfFrameAcross; j++)
		{
			FrameType frame;
			frame.x = static_cast<float>((j * dimensions.width) + offset.x) / static_cast<float>(bitmap.width);
			frame.y = static_cast<float>((i * dimensions.height) + offset.y) / static_cast<float>(bitmap.height);
			frame.width = (static_cast<float>(dimensions.width + offset.x) / static_cast<float>(bitmap.width)) + frame.x;
			frame.height = (static_cast<float>(dimensions.height + offset.y) / static_cast<float>(bitmap.height)) + frame.y;
			
			this->m_frameArray[(i * numberOfFrameAcross) + j] = frame;
		}
	}
}

void Sprite::SortFrameArray(int* framesOrder, int size)
{
	FrameType* tmp = new FrameType[size];

	for (int i = 0; i < size; i++)
	{
		tmp[i] = this->m_frameArray[framesOrder[i]];
	}
	
 	delete[] this->m_frameArray;
	this->m_frameArray = nullptr;
	this->m_frameArray = tmp;
}

Bitmap* Sprite::GetBitmap()
{
	return this->m_Bitmap;
}