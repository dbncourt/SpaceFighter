////////////////////////////////////////////////////////////////////////////////
// Filename: TextureShader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureShader.h"

TextureShader::TextureShader() : Shader()
{
}

TextureShader::TextureShader(const TextureShader& other)
{
}

TextureShader::~TextureShader()
{
}

bool TextureShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = TextureShader::InitializeShader(device, hwnd, L".\\..\\2DPlatformer\\CompiledShaders\\TextureVertexShader.cso", L".\\..\\2DPlatformer\\CompiledShaders\\TexturePixelShader.cso");
	if (!result)
	{
		return false;
	}

	return true;
}

void TextureShader::Shutdown()
{
	TextureShader::ShutdownShaders();
}

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = TextureShader::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	Shader::RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	HRESULT result;

	ShaderBufferType* vertexShaderBuffer = nullptr;
	ShaderBufferType* pixelShaderBuffer = nullptr;

	if (!Shader::LoadShader(vsFileName, &vertexShaderBuffer))
	{
		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->bufferPointer, vertexShaderBuffer->bufferSize, nullptr, &this->m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	if (!Shader::LoadShader(psFileName, &pixelShaderBuffer))
	{
		return false;
	}

	result = device->CreatePixelShader(pixelShaderBuffer->bufferPointer, pixelShaderBuffer->bufferSize, nullptr, &this->m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	if (!Shader::InitializeInputLayout(device, vertexShaderBuffer))
	{
		return false;
	}

	SAFE_DELETE(vertexShaderBuffer);
	SAFE_DELETE(pixelShaderBuffer);

	if (!Shader::InitializeConstantBuffers(device))
	{
		return false;
	}

	if (!Shader::InitializeSamplerState(device))
	{
		return false;
	}
	return true;
}

void TextureShader::ShutdownShaders()
{
	Shader::Shutdown();
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	
	result = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (FAILED(result))
	{
		return false;
	}

	MatrixBufferType* matrixBufferPtr = static_cast<MatrixBufferType*>(mappedSubresource.pData);

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	matrixBufferPtr->worldMatrix = worldMatrix;
	matrixBufferPtr->viewMatrix = viewMatrix;
	matrixBufferPtr->projectionMatrix = projectionMatrix;

	deviceContext->Unmap(this->m_matrixBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &this->m_matrixBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}
