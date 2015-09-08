////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

#include "System.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd){

	System* m_System;

	//Create the system object
	m_System = new System();
	if (!m_System)
	{
		return 0;
	}

	//Initialize and run the system object
	if (m_System->Initialize())
	{
		m_System->Run();
	}

	//Shutdown and release the system object
	m_System->Shutdown();
	delete m_System;
	m_System = nullptr;

	return 0;
}