// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDXApp.h"

int main()
{
	SDXApp app;

	if (app.Initialise("Test Window", 800, 600))
		app.Run();

	//system("PAUSE");
	return 0;
}

