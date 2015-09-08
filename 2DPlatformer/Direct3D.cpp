////////////////////////////////////////////////////////////////////////////////
// Filename: Direct3D.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Direct3D.h"


Direct3D::Direct3D()
{
	this->m_swapChain = nullptr;
	this->m_device = nullptr;
	this->m_deviceContext = nullptr;
	this->m_renderTargetView = nullptr;
	this->m_depthStencilBuffer = nullptr;
	this->m_depthEnabledStencilState = nullptr;
	this->m_depthDisabledStencilState = nullptr;
	this->m_depthStencilView = nullptr;
	this->m_rasterizerState = nullptr;
	this->m_alphaEnabledBlendingState = nullptr;
	this->m_alphaDisabledBlendingState = nullptr;
}


Direct3D::Direct3D(const Direct3D& other)
{
}


Direct3D::~Direct3D()
{
}

bool Direct3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;

	//Store the vsync setting
	this->m_vsync_enabled = vsync;

	//Create a DirectX graphics interface factory
	IDXGIFactory* factory;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	//Use the factory to create an adapter for the primary graphics interface (video card)
	IDXGIAdapter* adapter;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	//Enumerate the primary adapter output (monitor)
	IDXGIOutput* adapterOutput;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	//Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor)
	UINT numModes;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//Create a list to hold all the possible display modes for this monitor/video card combination
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	//Now go through all the display modes and find the one that matches the screen width and height
	//When a match is found, store the numerator and denominator of the refresh rate for that monitor
	UINT numerator;
	UINT denominator;
	for (UINT i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//Get the adapter (video card) description
	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	//Store the dedicated video card memory in megabytes
	this->m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//Convert the name of the video card to a character array and store it
	UINT stringLength;
	int error;
	error = wcstombs_s(&stringLength, this->m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//Release the display mode list
	delete[] displayModeList;
	displayModeList = nullptr;

	//Release the adapter output
	adapterOutput->Release();
	adapterOutput = nullptr;

	//Release the adapter
	adapter->Release();
	adapter = nullptr;

	//Release the factory
	factory->Release();
	factory = nullptr;

	// Create and Initialize the swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	//Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	//Set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//Set regular 32-bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Set the refresh rate of the back buffer
	if (this->m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//Set the handle for the window to render to
	swapChainDesc.OutputWindow = hwnd;

	//Turn multi sampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//Set to full screen or windowed mode
	swapChainDesc.Windowed = !fullscreen;

	//Set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//Don't set the advanced flags
	swapChainDesc.Flags = 0;

	//Set the feature level to DirectX 11
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	//Create the swap chain, Direct3D device, and Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &this->m_swapChain, &this->m_device, nullptr, &this->m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	//Get the pointer to the back buffer
	ID3D11Texture2D* backBufferPtr;
	result = this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	//Create the render target view with the back buffer pointer
	result = this->m_device->CreateRenderTargetView(backBufferPtr, nullptr, &this->m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}
	//Release pointer to the back buffer as we no longer need it
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	//Initialize the description of the depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//Set up the description of the depth buffer
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the texture for the depth buffer using the filled out description
	result = this->m_device->CreateTexture2D(&depthBufferDesc, nullptr, &this->m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the description of the stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	//Set up the description of the stencil state
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel if back facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the DisabledDepthStencilState
	result = this->m_device->CreateDepthStencilState(&depthStencilDesc, &this->m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//Set the DisabledDepthStencilState
	this->m_deviceContext->OMSetDepthStencilState(this->m_depthDisabledStencilState, 1);

	// Enable the Depth of the StencilState
	depthStencilDesc.DepthEnable = true;

	// Create the EnabledDepthStencilState
	result = this->m_device->CreateDepthStencilState(&depthStencilDesc, &this->m_depthEnabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	//Set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view
	result = this->m_device->CreateDepthStencilView(this->m_depthStencilBuffer, &depthStencilViewDesc, &this->m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//Bind the render target view and depth stencil buffer to the output render pipeline
	this->m_deviceContext->OMSetRenderTargets(1, &this->m_renderTargetView, this->m_depthStencilView);

	//Setup the raster description which will determine how and what polygons will be drawn
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer state from the description we just filled out
	result = this->m_device->CreateRasterizerState(&rasterizerDesc, &this->m_rasterizerState);
	if (FAILED(result))
	{
		return false;
	}

	//Now set the rasterizer state
	this->m_deviceContext->RSSetState(this->m_rasterizerState);

	//Setup the viewport for rendering
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//Create the viewport
	this->m_deviceContext->RSSetViewports(1, &viewport);

	// Create and clear the BlendState description
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	// Create the blend state using the description
	result = this->m_device->CreateBlendState(&blendStateDesc, &this->m_alphaEnabledBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description
	blendStateDesc.RenderTarget[0].BlendEnable = FALSE;

	result = this->m_device->CreateBlendState(&blendStateDesc, &this->m_alphaDisabledBlendingState);
	if (FAILED(result))
	{
		return false;
	}
	
	//Setup the projection matrix
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	//Create the projection matrix for 3D rendering
	D3DXMatrixPerspectiveFovLH(&this->m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	//Initialize the world matrix to the identity matrix
	D3DXMatrixIdentity(&this->m_worldMatrix);

	//Create an orthographic projection matrix for 2D rendering
	D3DXMatrixOrthoLH(&this->m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void Direct3D::Shutdown()
{

	//Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (this->m_swapChain)
	{
		this->m_swapChain->SetFullscreenState(FALSE, nullptr);
	}
	
	SAFE_RELEASE(this->m_alphaDisabledBlendingState);
	SAFE_RELEASE(this->m_alphaEnabledBlendingState);
	SAFE_RELEASE(this->m_rasterizerState);
	SAFE_RELEASE(this->m_depthStencilView);
	SAFE_RELEASE(this->m_depthEnabledStencilState);
	SAFE_RELEASE(this->m_depthDisabledStencilState);
	SAFE_RELEASE(this->m_depthStencilBuffer);
	SAFE_RELEASE(this->m_renderTargetView);
	SAFE_RELEASE(this->m_deviceContext);
	SAFE_RELEASE(this->m_device);
	SAFE_RELEASE(this->m_swapChain);
}

void Direct3D::BeginScene(D3DXCOLOR backBufferColor)
{
	//Setup the color to clear the buffer to
	float color[4] = { backBufferColor.r, backBufferColor.g, backBufferColor.b, backBufferColor.a };

	//Clear the back-buffer
	this->m_deviceContext->ClearRenderTargetView(this->m_renderTargetView, color);

	//Clear the depth buffer
	this->m_deviceContext->ClearDepthStencilView(this->m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndScene()
{
	//Present the back-buffer to the screen since rendering is complete
	if (this->m_vsync_enabled)
	{
		//Lock to screen refresh rate
		this->m_swapChain->Present(1, 0);
	}
	else
	{
		//Present as fast as possible
		this->m_swapChain->Present(0, 0);
	}
}

ID3D11Device* Direct3D::GetDevice()
{
	return this->m_device;
}

ID3D11DeviceContext* Direct3D::GetDeviceContext()
{
	return this->m_deviceContext;
}

void Direct3D::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = this->m_projectionMatrix;
}

void Direct3D::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = this->m_worldMatrix;
}

void Direct3D::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = this->m_orthoMatrix;
}

void Direct3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, this->m_videoCardDescription);
	memory = this->m_videoCardMemory;
}

void Direct3D::TurnOnZBuffer()
{
	this->m_deviceContext->OMSetDepthStencilState(this->m_depthEnabledStencilState, 1);
}

void Direct3D::TurnOffZBuffer()
{
	this->m_deviceContext->OMSetDepthStencilState(this->m_depthDisabledStencilState, 1);
}

void Direct3D::TurnOnAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending
	this->m_deviceContext->OMSetBlendState(this->m_alphaEnabledBlendingState, blendFactor, 0xffffffff);
}

void Direct3D::TurnOffAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending
	this->m_deviceContext->OMSetBlendState(this->m_alphaDisabledBlendingState, blendFactor, 0xffffffff);
}
