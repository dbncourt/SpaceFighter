////////////////////////////////////////////////////////////////////////////////
// Filename: Input.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//////////////
// INCLUDES //
//////////////
#include <dinput.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Input
////////////////////////////////////////////////////////////////////////////////
class Input
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth;
	int m_screenHeight;

public:
	Input();
	Input(const Input& other);
	~Input();

	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsSpaceBarPressed();
	bool IsUpArrowPressed();
	bool IsDownArrowPressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();

private:
	bool ReadKeyboard();
};
#endif