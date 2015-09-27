#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	this->m_ColorShader = nullptr;
}

BoxCollider::BoxCollider(const BoxCollider& other)
{
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT offset, BoxDimensionsType boxDimensions)
{
	bool result;

	this->m_offset = offset;
	this->m_boxDimensions = boxDimensions;

	result = Collider::Initialize(device, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the BoxCollider Object.", L"Error", MB_OK);
		return false;
	}

	this->m_ColorShader = new ColorShader();
	if (!this->m_ColorShader)
	{
		MessageBox(hwnd, L"Could not initialize the ColorShader Object.", L"Error", MB_OK);
		return false;
	}

	result = this->m_ColorShader->Initialize(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void BoxCollider::Shutdown()
{
	SAFE_SHUTDOWN(this->m_ColorShader);
	Collider::Shutdown();
}

bool BoxCollider::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = BoxCollider::UpdateBuffers(deviceContext, this->m_position);
	if (!result)
	{
		return false;
	}

	Collider::Render(deviceContext);

	result = this->m_ColorShader->Render(deviceContext, Collider::GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

void BoxCollider::Frame(const POINT position)
{
	this->m_position = POINT{ position.x + this->m_offset.x, position.y + this->m_offset.y };
}

const BoxCollider::BoxDimensionsType BoxCollider::GetBoxDimensions()
{
	return this->m_boxDimensions;
}

const BoxCollider::BoxVerticesType BoxCollider::GetBoxColliderVertices()
{
	BoxVerticesType boxColliderVertices;

	boxColliderVertices.x = this->m_position.x;
	boxColliderVertices.y = this->m_position.y;
	boxColliderVertices.width = this->m_boxDimensions.base;
	boxColliderVertices.height = this->m_boxDimensions.height;

	return boxColliderVertices;
}

bool BoxCollider::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	this->m_vertexCount = 4;
	this->m_indexCount = 5;

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

	for (int i = 0; i < this->m_vertexCount; i++)
	{
		indices[i] = i;
	}
	indices[this->m_indexCount - 1] = 0;

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

	indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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

bool BoxCollider::UpdateBuffers(ID3D11DeviceContext* deviceContext, const POINT position)
{
	HRESULT result;

	VertexType* vertices = new VertexType[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = D3DXVECTOR3(
		static_cast<float>(-this->m_screen.width / 2) + position.x,
		static_cast<float>(this->m_screen.height / 2) - position.y,
		1.0f);
	vertices[0].color = D3DXCOLOR(0.4f, 0.46f, 0.52f, 1.0f);

	vertices[1].position = D3DXVECTOR3(
		static_cast<float>(-this->m_screen.width / 2) + (position.x + this->m_boxDimensions.base),
		static_cast<float>(this->m_screen.height / 2) - position.y,
		1.0f);
	vertices[1].color = D3DXCOLOR(0.4f, 0.46f, 0.52f, 1.0f);

	vertices[2].position = D3DXVECTOR3(
		static_cast<float>(-this->m_screen.width / 2) + (position.x + this->m_boxDimensions.base),
		static_cast<float>(this->m_screen.height / 2) - (position.y + this->m_boxDimensions.height),
		1.0f);
	vertices[2].color = D3DXCOLOR(0.4f, 0.46f, 0.52f, 1.0f);

	vertices[3].position = D3DXVECTOR3(
		static_cast<float>(-this->m_screen.width / 2) + position.x,
		static_cast<float>(this->m_screen.height / 2) - (position.y + this->m_boxDimensions.height),
		1.0f);
	vertices[3].color = D3DXCOLOR(0.4f, 0.46f, 0.52f, 1.0f);

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