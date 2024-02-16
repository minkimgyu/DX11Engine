////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "DXEngine.h"


DXEngine::DXEngine()
{
	_Input = 0;
	_CoreSystem = 0;
}


DXEngine::DXEngine(const DXEngine& other)
{
}


DXEngine::~DXEngine()
{
}


bool DXEngine::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	_Input = new InputSystem;
	if (!_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = _Input->Initialize(_hinstance, _hwnd);
	if (!result)
	{
		MessageBox(_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	_CoreSystem = new CoreSystem();
	if(!_CoreSystem)
	{
		return false;
	}

	// Initialize the graphics object.
	result = _CoreSystem->Initialize(screenWidth, screenHeight, _hwnd);
	if(!result)
	{
		return false;
	}

	_CoreSystem->IsKeyPressedDelegate.Connect(_Input, &InputSystem::IsKeyPressed);

	return true;
}

void DXEngine::Shutdown()
{
	// Release the graphics object.
	if(_CoreSystem)
	{
		_CoreSystem->Shutdown();
		delete _CoreSystem;
		_CoreSystem = 0;
	}

	// Release the input object.
	if(_Input)
	{
		delete _Input;
		_Input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void DXEngine::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

		if (_Input->IsKeyPressed(DIK_ESCAPE) == true)
		{
			done = true;
		}
	}

	return;
}

bool DXEngine::Frame()
{
	bool result;

	result = _Input->Frame();
	if (!result)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = _CoreSystem->Frame();
	if(!result)
	{
		return false;
	}

	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	DXEngine* Engine;
	bool result;


	// Create the system object.
	Engine = new DXEngine;
	if (!Engine)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = Engine->Initialize();
	if (result)
	{
		Engine->Run();
	}

	// Shutdown and release the system object.
	Engine->Shutdown();
	delete Engine;
	Engine = 0;

	return 0;
}

LRESULT CALLBACK DXEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void DXEngine::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = _hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void DXEngine::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(_hwnd);
	_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(_applicationName, _hinstance);
	_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}