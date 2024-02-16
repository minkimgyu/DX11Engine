////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "CoreSystem.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "LogicSystem.h"
#include "RenderingSystem.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class DXEngine
{
public:
	DXEngine();
	DXEngine(const DXEngine&);
	~DXEngine();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR _applicationName;
	HINSTANCE _hinstance;
	HWND _hwnd;

	InputSystem* _Input;
	CoreSystem* _CoreSystem;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static DXEngine* ApplicationHandle = 0;


#endif