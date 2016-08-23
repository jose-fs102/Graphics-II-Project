////////////////////////////
// Jose Rivera
// Graphics II Project
// Start Date: 9/29/2015
// End Date:
////////////////////////////
#include "WinMainSystems.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdShow)
{
	WinMainSystems* wm_System;
	bool result;
	
	wm_System = new WinMainSystems;

	if (!wm_System)
		return 0;

	result = wm_System->WinMainInitialize(hInstance);

	if (result)
		wm_System->Run();

	wm_System->ShutDown();

	delete wm_System;

	wm_System = nullptr;

	return 1;
}