////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: InputSystem
////////////////////////////////////////////////////////////////////////////////
class InputSystem
{
public:
	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int& mouseX, int& mouseY);
	bool IsKeyPressed(int keyNumber);

private:
	bool ReadKeyboard();
	bool ReadMouse();

private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	DIMOUSESTATE _mouseState;
	DIMOUSESTATE _mouseLastState;
	int _mouseX, _mouseY;

	unsigned char m_keyboardState[256];
};

#endif