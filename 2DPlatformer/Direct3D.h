////////////////////////////////////////////////////////////////////////////////
// Filename: Direct3D.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _DIRECT3D_H_
#define _DIRECT3D_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

/////////////
// MACROS  //
/////////////
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Direct3D
////////////////////////////////////////////////////////////////////////////////
class Direct3D
{
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthEnabledStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterizerState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
	
	ID3D11BlendState* m_alphaEnabledBlendingState;
	ID3D11BlendState* m_alphaDisabledBlendingState;
public:
	Direct3D();
	Direct3D(const Direct3D& other);
	~Direct3D();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(D3DXCOLOR backBufferColor);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardName, int& memory);

	void TurnOnZBuffer();
	void TurnOffZBuffer();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
};

#endif