////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>

/////////////
// MACROS  //
/////////////
#define SAFE_SHUTDOWN(p) { if(p) { (p)->Shutdown(); delete(p); (p) = nullptr;}}
#define SAFE_DELETE(p) { if(p) { delete(p); (p) = nullptr;}}

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Direct3D.h"
#include "Camera.h"
#include "Game.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
private:
	Direct3D* m_Direct3D;
	Camera* m_Camera;
	Game* m_Game;

public:
	Graphics();
	Graphics(const Graphics& other);
	~Graphics();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame(InputHandler::ControlsType controls);

private:
	bool Render();
};
#endif