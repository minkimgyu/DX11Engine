////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "InputSystem.h"

InputSystem::InputSystem()
{
	_directInput = 0;
	_keyboard = 0;
	_mouse = 0;
}

InputSystem::InputSystem(const InputSystem& other)
{
}


InputSystem::~InputSystem()
{
}

void InputSystem::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
	return;
}

bool InputSystem::IsKeyPressed(int keyNumber)
{
	return m_keyboardState[keyNumber] & 0x80;
}

bool InputSystem::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = _keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputSystem::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputSystem::Shutdown()
{
	// Release the mouse.
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = 0;
	}

	// Release the keyboard.
	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (_directInput)
	{
		_directInput->Release();
		_directInput = 0;
	}

	return;
}

bool InputSystem::Frame()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	return true;
}

DIMOUSESTATE InputSystem::GetMouseState()
{
	return _mouseState;
}

bool InputSystem::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	// Initialize the location of the mouse on the screen.
	_mouseX = 0;
	_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// DISCL_BACKGROUND | DISCL_FOREGROUND
	// Set the cooperative level of the keyboard to not share with other programs.
	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = _keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = _mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}