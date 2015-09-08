////////////////////////////////////////////////////////////////////////////////
// Filename: Timer.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TIMER_H_
#define _TIMER_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer& other);
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	FLOAT m_ticksPerMs;
	INT64 m_startTime;
	FLOAT m_frameTime;
};

#endif