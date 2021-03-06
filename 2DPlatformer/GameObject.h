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
#include "Collider.h"

class GameObject
{
public:
	GameObject();
	GameObject(const GameObject& other);
	~GameObject();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, bool drawCollider) = 0;
	bool Initialize(ID3D11Device* device, HWND hwnd, Bitmap::DimensionType screen, WCHAR* spriteFileName, Bitmap::DimensionType bitmap, Bitmap::DimensionType sprite, int numberOfFramesAcross, int initialFrame, bool useTimer);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX wordMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void Move();
	void Move(const D3DXVECTOR2 vector);

	virtual void Frame(const InputHandler::ControlsType& controls);
	void SortFrameArray(const int* framesOrder, int size);

	void SetPosition(POINT position);
	const POINT GetPosition();
	const POINT* GetPositionAddress();

	void SetVelocity(const D3DXVECTOR2& velocity);
	const D3DXVECTOR2 GetVelocity();

	void SetActiveStatus(const bool status);
	bool GetActiveStatus();

	float GetMovementDelayTime();
	void ResetMovementDelayTime();

	float GetAnimationDelayTime();
	void ResetAnimationDelayTime();

	ID3D11Device* GetDevice();
	HWND GetHWND();

	Sprite* GetSprite();
	virtual Collider* GetCollider();

protected:
	Sprite* m_Sprite;
	Timer* m_Timer;
	POINT m_position;
	D3DXVECTOR2 m_velocity;
	Collider* m_Collider;
	bool m_active;
	float m_movementDelay;
	float m_animationDelay;
	bool m_drawCollider;

protected:
	const float MOVEMENT_DELAY = 16.0f;
};
#endif