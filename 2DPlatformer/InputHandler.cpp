////////////////////////////////////////////////////////////////////////////////
// Filename: InputHandler.h
////////////////////////////////////////////////////////////////////////////////
#include "InputHandler.h"


InputHandler::InputHandler()
{
	this->m_Controls = {
		false, //Up
		false, //Down
		false, //Right
		false, //Left
		false  //SpaceBar
	};
}

InputHandler::InputHandler(const InputHandler& other)
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::SetUp(bool key)
{
	this->m_Controls.up = key;
}

void InputHandler::SetDown(bool key)
{
	this->m_Controls.down = key;
}

void InputHandler::SetRight(bool key)
{
	this->m_Controls.right = key;
}

void InputHandler::SetLeft(bool key)
{
	this->m_Controls.left = key;
}

void InputHandler::SetSpaceBar(bool key)
{
	this->m_Controls.spaceBar = key;
}

InputHandler::ControlsType InputHandler::GetControls()
{
	return this->m_Controls;
}