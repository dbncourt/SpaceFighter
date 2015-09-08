////////////////////////////////////////////////////////////////////////////////
// Filename: Timer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Timer.h"

Timer::Timer()
{
}

Timer::Timer(const Timer& other)
{

}

Timer::~Timer()
{

}

bool Timer::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&this->m_frequency));
	if (this->m_frequency == 0.0f)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every milliseconds
	this->m_ticksPerMs = static_cast<float>(this->m_frequency / 1000);

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&this->m_startTime));
	
	return true;
}

void Timer::Frame()
{
	INT64 currentTime;
	FLOAT timeDifference;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentTime));

	timeDifference = static_cast<float>(currentTime - this->m_startTime);
	this->m_frameTime = timeDifference / this->m_ticksPerMs;
	this->m_startTime = currentTime;
}

float Timer::GetTime()
{
	return this->m_frameTime;
}
