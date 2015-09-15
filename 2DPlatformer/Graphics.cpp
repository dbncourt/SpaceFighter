////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Graphics.h"


Graphics::Graphics()
{
	this->m_Direct3D = nullptr;
	this->m_Camera = nullptr;
	this->m_Game = nullptr;
}

Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D Object
	this->m_Direct3D = new Direct3D();
	if (!this->m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D Object
	result = this->m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	this->m_Direct3D->TurnOffZBuffer();
	this->m_Direct3D->TurnOnAlphaBlending();

	// Create the Camera Object
	this->m_Camera = new Camera();
	if (!this->m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera
	this->m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -10.0f));

	// Create the Fighter Object
	this->m_Game = new Game();
	if (!this->m_Game)
	{
		return false;
	}

	// Initialize the Fighter Object
	result = this->m_Game->Initialize(this->m_Direct3D->GetDevice(), hwnd, Bitmap::DimensionType{ screenWidth, screenHeight });
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Game Object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Direct3D);
	SAFE_DELETE(this->m_Camera);
	SAFE_SHUTDOWN(this->m_Game);
}

bool Graphics::Frame(InputHandler::ControlsType controls)
{
	bool result;
	
	this->m_Game->Frame(controls);

	//Render the graphics scene
	result = Graphics::Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	bool result;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX orthoMatrix;

	//Clear the buffers to begin the scene
	this->m_Direct3D->BeginScene(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	this->m_Camera->Render();

	this->m_Direct3D->GetWorldMatrix(worldMatrix);
	this->m_Camera->GetViewMatrix(viewMatrix);
	this->m_Direct3D->GetOrthoMatrix(orthoMatrix);

	result = this->m_Game->Render(this->m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	
	//Present the rendered scene to the screen
	this->m_Direct3D->EndScene();

	return true;
}