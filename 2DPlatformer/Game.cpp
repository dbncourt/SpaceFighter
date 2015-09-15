#include "Game.h"

Game::Game()
{
	this->m_Fighter = nullptr;
	this->m_FighterFlame = nullptr;
	this->m_Bullets = nullptr;
	this->m_Stars = nullptr;
	this->m_Mines = nullptr;
}

Game::Game(const Game& other)
{
}

Game::~Game()
{
}

bool Game::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen)
{
	bool result;

	this->m_Fighter = new Fighter();
	if (!this->m_Fighter)
	{
		return false;
	}

	result = this->m_Fighter->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Fighter.", L"Error", MB_OK);
		return false;
	}

	this->m_FighterFlame = new FighterFlame();
	if (!this->m_FighterFlame)
	{
		return false;
	}

	result = this->m_FighterFlame->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the FighterFlame.", L"Error", MB_OK);
		return false;
	}

	this->m_Bullets = new BulletManager();
	if (!this->m_Bullets)
	{
		return false;
	}

	result = this->m_Bullets->Initialize(device, hwnd, screen);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the BulletManager.", L"Error", MB_OK);
		return false;
	}

	this->m_Stars = new StarManager();
	if (!this->m_Stars)
	{
		return false;
	}

	result = this->m_Stars->Initialize(device, hwnd, screen, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the StarManager.", L"Error", MB_OK);
		return false;
	}

	this->m_Mines = new MineManager();
	if (!this->m_Mines)
	{
		return false;
	}

	result = this->m_Mines->Initialize(device, hwnd, screen, 20);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the MineManager.", L"Error", MB_OK);
		return false;
	}
}

void Game::Shutdown()
{
	SAFE_SHUTDOWN(this->m_Fighter);
	SAFE_SHUTDOWN(this->m_FighterFlame);
	SAFE_SHUTDOWN(this->m_Bullets);
	SAFE_SHUTDOWN(this->m_Stars);
	SAFE_SHUTDOWN(this->m_Mines);
}

bool Game::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = this->m_Stars->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_FighterFlame->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_Fighter->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_Mines->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_Bullets->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

void Game::Frame(const InputHandler::ControlsType& controls)
{
	this->m_Fighter->Frame(controls);

	this->m_FighterFlame->SetPosition(this->m_Fighter->GetPosition());
	this->m_FighterFlame->Frame(controls);
	
	this->m_Bullets->SetRelativePosition(this->m_Fighter->GetPosition());
	this->m_Bullets->Frame(controls);

	this->m_Mines->Frame(controls);

	this->m_Stars->Frame(controls);
}
