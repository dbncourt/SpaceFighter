////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera();
	Camera(const Camera& other);
	~Camera();

	void SetPosition(D3DXVECTOR3 position);
	D3DXVECTOR3 GetPosition();
	void GetPosition(_Out_ D3DXVECTOR3& position);

	void SetRotation(D3DXVECTOR3 rotation);
	D3DXVECTOR3 GetRotation();
	void GetRotation(_Out_ D3DXVECTOR3& rotation);

	void Render();
	D3DXMATRIX GetViewMatrix();
	void GetViewMatrix(_Out_ D3DXMATRIX& viewMatrix);

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXMATRIX m_viewMatrix;
};

#endif