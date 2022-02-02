// This app is for testing/debugging SDX engine code

#include "stdafx.h"
#include "SDXTestApp.h"

int main()
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(242);

	SDXTestApp app;

	if (app.Initialise("Test Window", 800, 600))
		app.Run();

	//system("PAUSE");
	return 0;
}

