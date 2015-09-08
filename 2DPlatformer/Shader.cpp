////////////////////////////////////////////////////////////////////////////////
// Filename: Shader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Shader.h"

Shader::Shader()
{
	this->m_vertexShader = nullptr;
	this->m_pixelShader = nullptr;
	this->m_inputLayout = nullptr;
	this->m_matrixBuffer = nullptr;
	this->m_samplerState = nullptr;
}

Shader::Shader(const Shader& other)
{
}

Shader::~Shader()
{

}

void Shader::Shutdown()
{
	Shader::ShutdownShaders();
}

bool Shader::InitializeInputLayout(ID3D11Device* device, ShaderBufferType* vertexShaderBuffer)
{
	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC inputElementDescriptor[2];
	ZeroMemory(inputElementDescriptor, sizeof(inputElementDescriptor));

	inputElementDescriptor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescriptor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDescriptor[0].SemanticName = "POSITION";

	inputElementDescriptor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputElementDescriptor[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescriptor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputElementDescriptor[1].SemanticName = "TEXCOORD";

	UINT numElement = sizeof(inputElementDescriptor) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	result = device->CreateInputLayout(inputElementDescriptor, numElement, vertexShaderBuffer->bufferPointer, vertexShaderBuffer->bufferSize, &this->m_inputLayout);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Shader::InitializeConstantBuffers(ID3D11Device* device)
{
	HRESULT result;

	D3D11_BUFFER_DESC constantBufferDescriptor;
	ZeroMemory(&constantBufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	constantBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescriptor.ByteWidth = sizeof(MatrixBufferType);
	constantBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;

	result = device->CreateBuffer(&constantBufferDescriptor, nullptr, &this->m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Shader::InitializeSamplerState(ID3D11Device* device)
{
	HRESULT result;

	D3D11_SAMPLER_DESC samplerDescriptor;
	ZeroMemory(&samplerDescriptor, sizeof(D3D11_SAMPLER_DESC));

	samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.BorderColor[0] = 0.0f;
	samplerDescriptor.BorderColor[1] = 0.0f;
	samplerDescriptor.BorderColor[2] = 0.0f;
	samplerDescriptor.BorderColor[3] = 0.0f;
	samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescriptor.MaxAnisotropy = 1;
	samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDescriptor.MinLOD = 0.0f;
	samplerDescriptor.MipLODBias = 0.0f;

	result = device->CreateSamplerState(&samplerDescriptor, &this->m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Shader::ShutdownShaders()
{
	SAFE_RELEASE(this->m_samplerState);
	SAFE_RELEASE(this->m_matrixBuffer);
	SAFE_RELEASE(this->m_inputLayout);
	SAFE_RELEASE(this->m_pixelShader);
	SAFE_RELEASE(this->m_vertexShader);
}

bool Shader::LoadShader(WCHAR* compiledShaderFileName, _Out_ ShaderBufferType** shaderBuffer)
{
	if (!(*shaderBuffer))
	{
		*(shaderBuffer) = new ShaderBufferType;
	}

	std::wstring fileName = std::wstring(compiledShaderFileName);
	std::ifstream file(fileName.c_str(), std::ios::binary);
	if (file.bad())
	{
		return false;
	}

	file.seekg(0, std::ios::end);
	(*(shaderBuffer))->bufferSize = static_cast<UINT>(file.tellg());

	if ((*(shaderBuffer))->bufferSize > 0)
	{
		(*(shaderBuffer))->bufferPointer = new char[(*(shaderBuffer))->bufferSize];
		file.seekg(0, std::ios::beg);
		file.read((*(shaderBuffer))->bufferPointer, (*(shaderBuffer))->bufferSize);
	}

	file.close();

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(this->m_inputLayout);

	deviceContext->VSSetShader(this->m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(this->m_pixelShader, nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}