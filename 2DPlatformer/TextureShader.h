////////////////////////////////////////////////////////////////////////////////
// Filename: TextureShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

//////////////
// INCLUDES //
//////////////
#include "Shader.h"

class TextureShader : Shader
{
public:
	TextureShader();
	TextureShader(const TextureShader& other);
	~TextureShader();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) override;
	virtual void Shutdown() override;
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName) override;
	virtual void ShutdownShaders() override;
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
};

#endif