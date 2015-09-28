////////////////////////////////////////////////////////////////////////////////
// Filename: Score.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCORE_H_
#define _SCORE_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Number.h"

class Score
{
public:
	Score();
	Score(const Score& other);
	~Score();

	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, POINT position, int digits);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void IncrementScore();

	void SetActiveStatus(bool status);
	bool GetActiveStatus();

private:
	int* GetArrayOfDigits();

private:
	std::list<Number*> m_Numbers;

	bool m_activeStatus;
	int m_score;

	const int SCORE_INCREMENT = 5;
};
#endif