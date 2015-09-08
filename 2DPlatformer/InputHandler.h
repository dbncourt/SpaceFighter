////////////////////////////////////////////////////////////////////////////////
// Filename: InputHandler.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_HANDLER_H
#define _INPUT_HANDLER_H


class InputHandler
{
public:
	struct ControlsType
	{
		bool up;
		bool down;
		bool right;
		bool left;
		bool spaceBar;
	};

public:
	InputHandler();
	InputHandler(const InputHandler& other);
	~InputHandler();

	void SetUp(bool key);
	void SetDown(bool key);
	void SetRight(bool key);
	void SetLeft(bool key);
	void SetSpaceBar(bool key);

	ControlsType GetControls();

private:
	ControlsType m_Controls;
};
#endif