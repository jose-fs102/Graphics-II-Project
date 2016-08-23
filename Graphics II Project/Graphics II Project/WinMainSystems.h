#pragma once

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include "GraphicsClass.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Class used to handle creation of the window
class WinMainSystems
{
private:

	LPCWSTR appName;
	HINSTANCE application;
	HWND window;

public:

	WinMainSystems();
	WinMainSystems(const WinMainSystems&);
	~WinMainSystems();

	bool WinMainInitialize(HINSTANCE);
	void Run();
	void ShutDown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	bool Frame();
	void InitializeWindows(HINSTANCE, int, int);
	void ShutDownWindows();
};

// Function Prototype
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global
static WinMainSystems* appHandle = 0;