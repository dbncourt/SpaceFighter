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
	this->m_HealthManager = nullptr;
	this->m_Score = nullptr;
	this->m_ExhaustManager = nullptr;
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

	result = this->m_Mines->Initialize(device, hwnd, screen, 20, false);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the MineManager.", L"Error", MB_OK);
		return false;
	}
	this->m_Mines->SetActiveStatus(true);
	
	////////////////////////////////////////////////////////////////////////////////
	//									HEALTH MANAGER
	////////////////////////////////////////////////////////////////////////////////
	this->m_HealthManager = new HealthManager();
	if (!this->m_HealthManager)
	{
		return false;
	}

	result = this->m_HealthManager->Initialize(device, hwnd, screen, POINT{ 450, 10 });
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the HealthManager.", L"Error", MB_OK);
		return false;
	}
	this->m_HealthManager->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//									   SCORE
	////////////////////////////////////////////////////////////////////////////////
	this->m_Score = new Score();
	if (!this->m_Score)
	{
		return false;
	}

	result = this->m_Score->Initialize(device, hwnd, screen, POINT{ 300, 10 }, 4);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Score.", L"Error", MB_OK);
		return false;
	}
	this->m_Score->SetActiveStatus(true);

	////////////////////////////////////////////////////////////////////////////////
	//								 EXHAUST MANAGER
	////////////////////////////////////////////////////////////////////////////////
	this->m_ExhaustManager = new ExhaustManager();
	if (!this->m_ExhaustManager)
	{
		return false;
	}

	result = this->m_ExhaustManager->Initialize(device, hwnd, screen);
	if (!result)
	{
		return false;
	}
	this->m_ExhaustManager->SetActiveStatus(true);

	return true;
}

void Game::Shutdown()
{
 	SAFE_SHUTDOWN(this->m_Fighter);
 	SAFE_SHUTDOWN(this->m_FighterFlame);
 	SAFE_SHUTDOWN(this->m_Bullets);
 	SAFE_SHUTDOWN(this->m_Stars);
	SAFE_SHUTDOWN(this->m_Mines);
	SAFE_SHUTDOWN(this->m_HealthManager);
	SAFE_SHUTDOWN(this->m_Score);
	SAFE_SHUTDOWN(this->m_ExhaustManager);
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

	this->m_ExhaustManager->Frame(controls, POINT{
		this->m_Fighter->GetPosition().x + 10,
		this->m_Fighter->GetPosition().y + 54
	});
 
 	Game::CheckCollisions();
}

void Game::CheckCollisions()
{
	bool collision = false;
	for (std::list<GameObject*>::iterator mineIterator = this->m_Mines->GetListBegin(); mineIterator != this->m_Mines->GetListEnd(); collision = false)
	{
		GameObject* mine = *mineIterator;
		for (std::list<GameObject*>::iterator bulletIterator = this->m_Bullets->GetListBegin(); bulletIterator != this->m_Bullets->GetListEnd();)
		{
			GameObject* bullet = *bulletIterator;
			if (Game::CheckCircleCircleCollision(dynamic_cast<CircleCollider*>(bullet->GetCollider()), dynamic_cast<CircleCollider*>(mine->GetCollider())))
			{
				bulletIterator = this->m_Bullets->NotifyCollision(bulletIterator);
				mineIterator = this->m_Mines->NotifyCollision(mineIterator);
				this->m_Score->IncrementScore();
				collision = true;
				break;
			}
			else
			{
				bulletIterator++;
			}
		}
		if (!collision)
		{
			if (Game::CheckCircleRectangleCollision(dynamic_cast<BoxCollider*>(this->m_Fighter->GetCollider()), dynamic_cast<CircleCollider*>(mine->GetCollider())))
			{
				mineIterator = this->m_Mines->NotifyCollision(mineIterator);
				this->m_HealthManager->DecrementHealth();
			}
			else
			{
				mineIterator++;
			}
		}
	}
}

bool Game::CheckCircleCircleCollision(CircleCollider* gO1Collider, CircleCollider* gO2Collider)
{
	//Circle - Circle Collision = sqrt((h2 - h1)^2 + (k2 - k1)^2) <= (r1 + r2)^2
	float a = (gO2Collider->GetCenter().x - gO1Collider->GetCenter().x);
	float b = (gO2Collider->GetCenter().y - gO1Collider->GetCenter().y);
	float r = (gO2Collider->GetRadius() + gO1Collider->GetRadius());

	return (((a*a) + (b*b)) <= r*r);
}

bool Game::CheckCircleRectangleCollision(BoxCollider* gO1Collider, CircleCollider* gO2Collider)
{
	BoxCollider::BoxVerticesType boxColliderVertices = gO1Collider->GetBoxColliderVertices();

	float halfWidth = boxColliderVertices.width / 2;
	float halfHeigh = boxColliderVertices.height / 2;

	POINTF rectangleCenter = POINTF{
		boxColliderVertices.x + halfWidth,
		boxColliderVertices.y + halfHeigh,
	};

	float dx = fabs(gO2Collider->GetCenter().x - rectangleCenter.x);
	float dy = fabs(gO2Collider->GetCenter().y - rectangleCenter.y);

	if (dx > (gO2Collider->GetRadius() + halfWidth) || dy > (gO2Collider->GetRadius() + halfHeigh))
	{
		return false;
	}

	POINTF circleDistance = POINTF{
		fabs(gO2Collider->GetCenter().x - boxColliderVertices.x - boxColliderVertices.width),
		fabs(gO2Collider->GetCenter().y - boxColliderVertices.y - boxColliderVertices.height)
	};

	if (circleDistance.x <= halfWidth)
	{
		return true;
	}
	else if (circleDistance.y <= halfHeigh)
	{
		return true;
	}

	float cornerDistanceSq = powf(circleDistance.x - halfWidth, 2) + powf(circleDistance.y - halfHeigh, 2);

	return (cornerDistanceSq <= powf(gO2Collider->GetRadius(), 2));
}

bool Game::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	result = this->m_Stars->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_ExhaustManager->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
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

	result = this->m_HealthManager->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = this->m_Score->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}