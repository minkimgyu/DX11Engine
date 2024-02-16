////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Renderer_H_
#define _Renderer_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3dModule.h"
#include "Component.h"

class Renderer : public Component
{
public:
	virtual bool Initialize(ID3D11Device*) = 0;
	virtual void Render(ID3D11DeviceContext*) = 0;
	virtual int GetIndexCount() = 0;
	virtual void Shutdown() = 0;
};

#endif