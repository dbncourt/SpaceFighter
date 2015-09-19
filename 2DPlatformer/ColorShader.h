////////////////////////////////////////////////////////////////////////////////
// Filename: ColorShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLOR_SHADER_H_
#define _COLOR_SHADER_H_

//////////////
// INCLUDES //
//////////////
#include "Shader.h"

class ColorShader : public Shader
{
public:
	ColorShader();
	ColorShader(const ColorShader& other);
	~ColorShader();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) override;
	virtual void Shutdown() override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	
private:
	virtual bool InitializeInputLayout(ID3D11Device* device, ShaderBufferType* vertexShaderBuffer) override;
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName) override;
	virtual void ShutdownShaders() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) override;
};
#endif