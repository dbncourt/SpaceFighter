////////////////////////////////////////////////////////////////////////////////
// Filename: CircleCollider.cpp
////////////////////////////////////////////////////////////////////////////////
#include "CircleCollider.h"

CircleCollider::CircleCollider() : Collider()
{
	this->m_ColorShader = nullptr;
}

CircleCollider::CircleCollider(const CircleCollider& other)
{
}

CircleCollider::~CircleCollider()
{
}

bool CircleCollider::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT offset, float radius)
{
	bool result;

	this->m_offset = offset;
	this->m_radius = radius;
	this->m_screen = screen;

	result = Collider::Initialize(device, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the CircleCollider Object.", L"Error", MB_OK);
		return false;
	}

	this->m_ColorShader = new ColorShader();
	if (!this->m_ColorShader)
	{
		return false;
	}

	result = this->m_ColorShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ColorShader Object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool CircleCollider::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;

	this->m_vertexCount = 24;
	this->m_indexCount = 25;

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

bool CircleCollider::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = CircleCollider::UpdateBuffers(deviceContext, this->m_center);
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

void CircleCollider::Shutdown()
{
	SAFE_SHUTDOWN(this->m_ColorShader);
	Collider::Shutdown();
}

bool CircleCollider::UpdateBuffers(ID3D11DeviceContext* deviceContext, POINT center)
{
	HRESULT result;

	VertexType* vertices = new VertexType[this->m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	float angleDivision = static_cast<float>((2 * D3DX_PI) / this->m_vertexCount);

	for (int i = 0; i < this->m_vertexCount; i++)
	{
		float angle = i * angleDivision;

		vertices[i].position = D3DXVECTOR3(
			static_cast<float>(-this->m_screen.width / 2) + (static_cast <float>(this->m_center.x + (this->m_radius * cos(angle)))),
			static_cast<float>(this->m_screen.height / 2) - (static_cast <float>(this->m_center.y + (this->m_radius * sin(angle)))),
			1.0f
			);
		vertices[i].color = D3DXCOLOR(0.4f, 0.46f, 0.52f, 1.0f);

	}

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

void CircleCollider::Frame(const POINT position)
{
	this->m_center = CircleCollider::GetCenterFromPosition(position);
}

const POINT CircleCollider::GetCenter()
{
	return this->m_center;
}

const POINT CircleCollider::GetCenterFromPosition(const POINT position)
{
	POINT center{
		position.x + this->m_offset.x,
		position.y + this->m_offset.y
	};

	return center;
}

const float CircleCollider::GetRadius()
{
	return this->m_radius;
}
