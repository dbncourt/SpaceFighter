////////////////////////////////////////////////////////////////////////////////
// Filename: Input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Input.h"


Input::Input()
{
	this->m_directInput = nullptr;
	this->m_keyboard = nullptr;
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

bool Input::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	//Initialize the main direct input interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&this->m_directInput, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//Initialize the direct input interface for the keyboard
	result = this->m_directInput->CreateDevice(GUID_SysKeyboard, &this->m_keyboard, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	//Set the data format. In this case since it is a keyboard we can use the predefined data format
	result = this->m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//Set the cooperative level of the keyboard to not share with other programs
	result = this->m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	//Now acquire the keyboard
	result = this->m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::Shutdown()
{
	//Release the keyboard
	if (this->m_keyboard)
	{
		this->m_keyboard->Unacquire();
		this->m_keyboard->Release();
		this->m_keyboard = nullptr;
	}

	//Re;ease the main interface to direct input
	if (this->m_directInput)
	{
		this->m_directInput->Release();
		this->m_directInput = nullptr;
	}
}

bool Input::Frame()
{
	bool result;

	//Read the current state of the keyboard
	result = Input::ReadKeyboard();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	//Read the keyboard device
	result = this->m_keyboard->GetDeviceState(sizeof(this->m_keyboardState), (LPVOID)&this->m_keyboardState);
	if (FAILED(result))
	{
		//If the keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			this->m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::IsEscapePressed()
{
	//Do a bitwise AND on the keyboard state to check if the escape key is currently being pressed
	if (this->m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsSpaceBarPressed()
{
	//Do a bitwise AND on the keyboard state to check if the space bar key is currently being pressed
	if (this->m_keyboardState[DIK_SPACE] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsUpArrowPressed()
{
	//Do a bitwise AND on the keyboard state to check if the up arrow key is currently being pressed
	if (this->m_keyboardState[DIK_UPARROW] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsDownArrowPressed()
{
	//Do a bitwise AND on the keyboard state to check if the down arrow key is currently being pressed
	if (this->m_keyboardState[DIK_DOWNARROW] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsLeftArrowPressed()
{
	//Do a bitwise AND on the keyboard state to check if the left arrow key is currently being pressed
	if (this->m_keyboardState[DIK_LEFTARROW] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsRightArrowPressed()
{
	//Do a bitwise AND on the keyboard state to check if the right arrow key is currently being pressed
	if (this->m_keyboardState[DIK_RIGHTARROW] & 0x80)
	{
		return true;
	}
	return false;
}