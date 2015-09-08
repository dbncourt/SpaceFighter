////////////////////////////////////////////////////////////////////////////////
// Filename: Bitmap.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Bitmap.h"

Bitmap::Bitmap()
{
	this->m_vertexBuffer = nullptr;
	this->m_indexBuffer = nullptr;
	this->m_Texture = nullptr;
}

Bitmap::Bitmap(const Bitmap& other)
{
}

Bitmap::~Bitmap()
{
}

bool Bitmap::Initialize(ID3D11Device* device, DimensionType screen, WCHAR* textureFileName, DimensionType bitmap)
{
	bool result;

	this->m_screen = screen;
	this->m_previousPosition = { -1, -1 };
	this->m_bitmap = bitmap;

	result = Bitmap::InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = Bitmap::LoadTexture(device, textureFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Bitmap::Shutdown()
{
	Bitmap::ShutdownBuffers();

	Bitmap::ReleaseTexture();
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, POINT position, D3DXVECTOR4 textureCoordinates)
{
	bool result;

	result = Bitmap::UpdateBuffers(deviceContext, position, textureCoordinates);
	if (!result)
	{
		return false;
	}

	Bitmap::RenderBuffers(deviceContext);

	return true;
}

UINT Bitmap::GetIndexCount()
{
	return this->m_indexCount;
}

void Bitmap::GetIndexCount(_Out_ UINT& indexCount)
{
	indexCount = this->m_indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
	return this->m_Texture->GetTexture();
}

void Bitmap::GetTexture(_Out_ ID3D11ShaderResourceView*& texture)
{
	texture = this->m_Texture->GetTexture();
}

Bitmap::DimensionType Bitmap::GetScreenDimensions()
{
	return this->m_screen;
}

Bitmap::DimensionType Bitmap::GetBitmapDimensions()
{
	return this->m_bitmap;
}

bool Bitmap::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	this->m_vertexCount = 6;
	this->m_indexCount = this->m_vertexCount;

	VertexType* vertices = new VertexType[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	UINT* indices = new UINT[this->m_indexCount];
	if (!indices)
	{
		return false;
	}

	ZeroMemory(vertices, sizeof(VertexType) * this->m_vertexCount);

	for (UINT i = 0; i < this->m_indexCount; i++)
	{
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * this->m_vertexCount;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));

	vertexData.pSysMem = vertices;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = sizeof(UINT) * this->m_indexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));

	indexData.pSysMem = indices;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Bitmap::ShutdownBuffers()
{
	SAFE_RELEASE(this->m_vertexBuffer);
	SAFE_RELEASE(this->m_indexBuffer);
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, POINT position, D3DXVECTOR4 textureCoordinates)
{
	HRESULT result;

// 	if (this->m_previousPosition.x == position.x && this->m_previousPosition.y == position.y)
// 	{
// 		return true;
// 	}

	this->m_previousPosition = position;
	RECT imagePosition;

	//Calculate the screen coordinates of the left side of the bitmap
	imagePosition.left = static_cast<float>(-this->m_screen.width / 2) + static_cast<float>(this->m_previousPosition.x);

	//Calculate the screen coordinates of the right side of the bitmap
	imagePosition.right = imagePosition.left + static_cast<float>(this->m_bitmap.width);

	//Calculate the screen coordinates of the top of the bitmap
	imagePosition.top = static_cast<float>(this->m_screen.height / 2) - static_cast<float>(this->m_previousPosition.y);

	//Calculate the screen coordinates of the bottom of the bitmap
	imagePosition.bottom = imagePosition.top - static_cast<float>(this->m_bitmap.height);

	//Create the vertex array
	VertexType* vertices = new VertexType[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	//Load the vertex array with data
	//First triangle
	vertices[0].position = D3DXVECTOR3(imagePosition.left, imagePosition.top, 0.0f);//Top Left
	vertices[0].texture = D3DXVECTOR2(textureCoordinates.x, textureCoordinates.y);

	vertices[1].position = D3DXVECTOR3(imagePosition.right, imagePosition.bottom, 0.0f);//Bottom Right
	vertices[1].texture = D3DXVECTOR2(textureCoordinates.z, textureCoordinates.w);

	vertices[2].position = D3DXVECTOR3(imagePosition.left, imagePosition.bottom, 0.0f);//Bottom Left
	vertices[2].texture = D3DXVECTOR2(textureCoordinates.x, textureCoordinates.w);

	//Second triangle
	vertices[3].position = D3DXVECTOR3(imagePosition.left, imagePosition.top, 0.0f);//Top Left
	vertices[3].texture = D3DXVECTOR2(textureCoordinates.x, textureCoordinates.y);

	vertices[4].position = D3DXVECTOR3(imagePosition.right, imagePosition.top, 0.0f);//Top Right
	vertices[4].texture = D3DXVECTOR2(textureCoordinates.z, textureCoordinates.y);

	vertices[5].position = D3DXVECTOR3(imagePosition.right, imagePosition.bottom, 0.0f);//Bottom Right
	vertices[5].texture = D3DXVECTOR2(textureCoordinates.z, textureCoordinates.w);

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	result = deviceContext->Map(this->m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (FAILED(result))
	{
		return false;
	}

	VertexType* verticesPtr = static_cast<VertexType*>(mappedSubresource.pData);
	memcpy(verticesPtr, static_cast<void*>(vertices), sizeof(VertexType) * this->m_vertexCount);

	deviceContext->Unmap(this->m_vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::LoadTexture(ID3D11Device* device, WCHAR* textureFileName)
{
	bool result;

	this->m_Texture = new Texture();
	if (!this->m_Texture)
	{
		return false;
	}

	result = this->m_Texture->Initialize(device, textureFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Bitmap::ReleaseTexture()
{
	SAFE_SHUTDOWN(this->m_Texture);
}