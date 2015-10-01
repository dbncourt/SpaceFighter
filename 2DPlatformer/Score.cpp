////////////////////////////////////////////////////////////////////////////////
// Filename: Score.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Score.h"

Score::Score()
{
	this->m_score = 0;
}

Score::Score(const Score& other)
{
}

Score::~Score()
{
}

bool Score::Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT position, int digits)
{
	bool result;

	for (int i = 0; i < digits; i++)
	{
		Number* number = new Number();
		if (!number)
		{
			return false;
		}

		result = number->Initialize(device, hwnd, screen, false);
		if (!result)
		{
			return false;
		}

		number->SetPosition(POINT{
			position.x + (17 * i),
			position.y
		});
		this->m_Numbers.push_back(number);
	}

	return true;
}

void Score::Shutdown()
{
	for (GameObject* number : this->m_Numbers)
	{
		SAFE_SHUTDOWN(number);
	}
	this->m_Numbers.clear();
}

bool Score::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (this->m_activeStatus)
	{
		bool result;

		int* digits = Score::GetArrayOfDigits();
		int i = this->m_Numbers.size() - 1;
		for (GameObject* number : this->m_Numbers)
		{
			number->GetSprite()->SetCurrentFrame(digits[i--]);
			result = number->Render(deviceContext, wordMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
	}
	return true;
}

void Score::IncrementScore()
{
	if (this->m_score + SCORE_INCREMENT < pow(10, this->m_Numbers.size()))
	{
		this->m_score += SCORE_INCREMENT;
	}
	else
	{
		this->m_score = pow(10, this->m_Numbers.size()) - 1;
	}
}

void Score::SetActiveStatus(bool status)
{
	this->m_activeStatus = status;
}

bool Score::GetActiveStatus()
{
	return this->m_activeStatus;
}

int* Score::GetArrayOfDigits()
{
	int* arrayOfDigits = nullptr;
	if (this->m_Numbers.size() > 0)
	{
		arrayOfDigits = new int[this->m_Numbers.size()];

		int tempScore = this->m_score;
		for (int i = this->m_Numbers.size() - 1; i > 0; i--)
		{
			int divisor = (pow(10, i));
			arrayOfDigits[i] = tempScore / divisor;
			tempScore %= divisor;
		}
		arrayOfDigits[0] = tempScore;
	}
	return arrayOfDigits;
}
