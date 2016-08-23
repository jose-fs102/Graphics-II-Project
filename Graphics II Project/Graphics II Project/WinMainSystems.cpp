#include "WinMainSystems.h"

GraphicsClass* _graphics;

WinMainSystems::WinMainSystems()
{
	_graphics = new GraphicsClass;
}

WinMainSystems::WinMainSystems(const WinMainSystems& other)
{
	// Empty for now
}

WinMainSystems::~WinMainSystems()
{
	// Clean up happens in shutdown
}

bool WinMainSystems::WinMainInitialize(HINSTANCE hInstance)
{
	InitializeWindows(hInstance, SCREEN_WIDTH, SCREEN_HEIGHT);

	return true;
}

void WinMainSystems::ShutDown()
{
	if (_graphics)
	{
		_graphics->ShutDown();
		delete _graphics;
		_graphics = 0;
	}

	ShutDownWindows();

	return;
}

void WinMainSystems::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();

			if (!result)
				done = true;
		}
	}
}

bool WinMainSystems::Frame()
{
	bool result;

	if (GetAsyncKeyState(VK_ESCAPE))
		return false;

	result = _graphics->Frame();

	if (!result)
		return false;

	return true;
}

LRESULT CALLBACK WinMainSystems::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void WinMainSystems::InitializeWindows(HINSTANCE hInstance, int screenWidth, int screenHeight)
{
	WNDCLASSEX windowsClass;
	ZeroMemory(&windowsClass, sizeof(windowsClass));

	appHandle = this;

	application = hInstance;
	appName = L"Graphics II Project";

	windowsClass.lpfnWndProc = WndProc;
	windowsClass.hInstance = application;
	windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowsClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	windowsClass.lpszClassName = appName;
	windowsClass.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&windowsClass);

	RECT window_size = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW, /*& ~(WS_THICKFRAME | WS_MAXIMIZEBOX),*/
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, application, this);

	_graphics->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, window);

	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);

	return;
}

void WinMainSystems::ShutDownWindows()
{
	DestroyWindow(window);
	window = NULL;

	UnregisterClass(appName, application);
	application = NULL;

	appHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_SIZE:
	{
		_graphics->GetDirect3DPtr()->Resize(LOWORD(lParam), HIWORD(lParam));
	}

	default:
		return appHandle->MessageHandler(hwnd, uMessage, wParam, lParam);
	}
}