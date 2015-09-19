////////////////////////////////////////////////////////////////////////////////
// Filename: ColorShader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ColorShader.h"

ColorShader::ColorShader() : Shader()
{
}

ColorShader::ColorShader(const ColorShader& other)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = ColorShader::InitializeShader(device, hwnd, L".\\..\\2DPlatformer\\CompiledShaders\\ColorVertexShader.cso", L".\\..\\2DPlatformer\\CompiledShaders\\ColorPixelShader.cso");
	if (!result)
	{
		return false;
	}

	return true;
}

void ColorShader::Shutdown()
{
	ColorShader::ShutdownShaders();
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = ColorShader::SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	ColorShader::RenderShader(deviceContext, indexCount);
	
	return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
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

	if (!ColorShader::InitializeInputLayout(device, vertexShaderBuffer))
	{
		return false;
	}

	SAFE_DELETE(vertexShaderBuffer);
	SAFE_DELETE(pixelShaderBuffer);

	if (!Shader::InitializeConstantBuffers(device))
	{
		return false;
	}

	return true;
}

void ColorShader::ShutdownShaders()
{
	Shader::Shutdown();
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
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

	return true;
}

bool ColorShader::InitializeInputLayout(ID3D11Device* device, ShaderBufferType* vertexShaderBuffer)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC inputElementDescriptor[2];
	ZeroMemory(inputElementDescriptor, sizeof(inputElementDescriptor));

	inputElementDescriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDescriptor[0].SemanticName = "POSITION";

	inputElementDescriptor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputElementDescriptor[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescriptor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDescriptor[1].SemanticName = "COLOR";

	UINT numElements = sizeof(inputElementDescriptor) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	result = device->CreateInputLayout(inputElementDescriptor, numElements, vertexShaderBuffer->bufferPointer, vertexShaderBuffer->bufferSize, &this->m_inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(this->m_inputLayout);

	deviceContext->VSSetShader(this->m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(this->m_pixelShader, nullptr, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
