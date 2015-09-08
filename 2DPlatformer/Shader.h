////////////////////////////////////////////////////////////////////////////////
// Filename: Shader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADER_H_
#define _SHADER_H_

/////////////
// MACROS  //
/////////////
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr;}}
#define SAFE_DELETE(p) { if(p) {delete (p); (p) = nullptr;}}
#define SAFE_SHUTDOWN(p) { if(p) { (p)->Shutdown(); delete (p); (p) = nullptr;}}

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <string>
using namespace std;

class Shader
{
protected:
	struct MatrixBufferType
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
	};

	struct ShaderBufferType
	{
		char* bufferPointer;
		ULONG bufferSize;
	};

public:
	Shader();
	Shader(const Shader& other);
	~Shader();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName) = 0;
	virtual bool InitializeInputLayout(ID3D11Device* device, ShaderBufferType* vertexShaderBuffer);
	virtual bool InitializeConstantBuffers(ID3D11Device* device);
	virtual bool InitializeSamplerState(ID3D11Device* device);
	virtual void ShutdownShaders();
	bool LoadShader(WCHAR* compiledShaderFileName, _Out_ ShaderBufferType** shaderBuffer);
	
	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_samplerState;
};

#endif