////////////////////////////////////////////////////////////////////////////////
// Filename: Game.cpp
////////////////////////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////////////////////////
	//									FIGHTER
	////////////////////////////////////////////////////////////////////////////////
	this->m_Fighter = new Fighter();
	if (!this->m_Fighter)
	{
		return false;
	}

	result = this->m_Fighter->Initialize(device, hwnd, screen, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Fighter.", L"Error", MB_OK);
		return false;
	}
	this->m_Fighter->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//								FIGHTER FLAME
	////////////////////////////////////////////////////////////////////////////////
	this->m_FighterFlame = new FighterFlame();
	if (!this->m_FighterFlame)
	{
		return false;
	}

	result = this->m_FighterFlame->Initialize(device, hwnd, screen, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the FighterFlame.", L"Error", MB_OK);
		return false;
	}
	this->m_FighterFlame->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//								BULLET MANAGER
	////////////////////////////////////////////////////////////////////////////////
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
	this->m_Bullets->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//								STAR MANAGER
	////////////////////////////////////////////////////////////////////////////////
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
	this->m_Stars->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//									MINE MANAGER
	////////////////////////////////////////////////////////////////////////////////
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
	this->m_Mines->SetActiveStatus(true);
}

void Game::Shutdown()
{
 	SAFE_SHUTDOWN(this->m_Fighter);
 	SAFE_SHUTDOWN(this->m_FighterFlame);
 	SAFE_SHUTDOWN(this->m_Bullets);
 	SAFE_SHUTDOWN(this->m_Stars);
	SAFE_SHUTDOWN(this->m_Mines);
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
 
 	Game::CheckCollisions();
}

void Game::CheckCollisions()
{
	bool collision = false;
	for (std::list<GameObject*>::iterator bulletIterator = this->m_Bullets->GetListBegin(); bulletIterator != this->m_Bullets->GetListEnd(); collision = false)
	{
		GameObject* bullet = *bulletIterator;
		for (GameObject* mine : this->m_Mines->GetList())
		{
			if (Game::AreCircleCollidersColliding(bullet, mine))
			{
				bulletIterator = this->m_Bullets->NotifyCollision(bulletIterator);
				this->m_Mines->NotifyCollision(&mine);
				collision = true;
				break;
			}
		}
		if (!collision)
		{
			bulletIterator++;
		}
	}
}

bool Game::AreCircleCollidersColliding(GameObject* gO1, GameObject* gO2)
{
	CircleCollider* gO1Collider = dynamic_cast<CircleCollider*>(gO1->GetCollider());
	CircleCollider* gO2Collider = dynamic_cast<CircleCollider*>(gO2->GetCollider());

	//Circle - Circle Collision = sqrt((h2 - h1)^2 + (k2 - k1)^2) <= (r1 + r2)^2
	float a = (gO2Collider->GetCenter().x - gO1Collider->GetCenter().x);
	float b = (gO2Collider->GetCenter().y - gO1Collider->GetCenter().y);
	float r = (gO2Collider->GetRadius() + gO1Collider->GetRadius());

	return (((a*a) + (b*b)) <= r*r);
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