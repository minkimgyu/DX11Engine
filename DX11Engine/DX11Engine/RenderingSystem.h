////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3dModule.h"
#include "Camera.h"
#include "GameObject.h"
#include "ColorShader.h"
#include "ModelRenderer.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class RenderingSystem
{
public:
	RenderingSystem(Camera*, list<GameObject*>*);
	RenderingSystem(const RenderingSystem&);
	~RenderingSystem();

	void InitializeRenderer(Renderer* renderer);

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	list<GameObject*>* _gameObjectsFromCore;

	D3dModule* _D3dModule;

	Camera* _CameraFromSystem;
	ColorShader* _ColorShader;
};

#endif