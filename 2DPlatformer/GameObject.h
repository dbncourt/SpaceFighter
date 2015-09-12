////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Sprite.h"
#include "InputHandler.h"
#include "Timer.h"

class GameObject : public Sprite
{
public:
	GameObject();
	GameObject(const GameObject& other);
	~GameObject();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen) = 0;
	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* spriteFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, int numberOfFramesAcross, int initialFrame, bool useTimer);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Move();
	void Move(const D3DXVECTOR2 vector);

	virtual void Frame(const InputHandler::ControlsType& controls);
	void SortFrameArray(const int* framesOrder, int size);

	void SetPosition(const POINT& position);
	const POINT GetPosition();

	void SetVelocity(const D3DXVECTOR2& velocity);
	const D3DXVECTOR2 GetVelocity();

	void SetStatus(const bool status);
	bool GetStatus();

	float GetMovementDelayTime();
	void ResetMovementDelayTime();

	float GetAnimationDelayTime();
	void ResetAnimationDelayTime();

	Sprite* GetSprite();

protected:
	ID3D11Device* m_device;
	HWND m_hwnd;
	Sprite* m_Sprite;
	Timer* m_Timer;
	POINT m_position;
	D3DXVECTOR2 m_velocity;
	bool m_active;
	float m_movementDelay;
	float m_animationDelay;
};
#endif