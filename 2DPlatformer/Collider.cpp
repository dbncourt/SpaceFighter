////////////////////////////////////////////////////////////////////////////////
// Filename: Collider.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Collider.h"

Collider::Collider()
{
	this->m_vertexBuffer = nullptr;
	this->m_indexBuffer = nullptr;
}

Collider::Collider(const Collider& other)
{
}

Collider::~Collider()
{
}

bool Collider::Initialize(ID3D11Device* device, Bitmap::DimensionType screen)
{
	bool result;

	this->m_screen = screen;

	result = this->InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void Collider::Shutdown()
{
	Collider::ShutdownBuffers();
}

void Collider::Render(ID3D11DeviceContext* deviceContext)
{
	this->RenderBuffers(deviceContext);
}

int Collider::GetIndexCount()
{
	return this->m_indexCount;
}

void Collider::ShutdownBuffers()
{
	SAFE_RELEASE(this->m_indexBuffer);
	SAFE_RELEASE(this->m_vertexBuffer);
}

void Collider::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}