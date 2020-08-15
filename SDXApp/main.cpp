// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDXApp.h"

int main()
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(242);

	SDXApp app;

	if (app.Initialise("Test Window", 800, 600))
		app.Run();

	//system("PAUSE");
	return 0;
}

