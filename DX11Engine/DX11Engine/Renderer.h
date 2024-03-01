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

#include<vector>

using namespace std;

class Renderer : public Component
{
public:
	Renderer();
	~Renderer();

public:
	virtual bool Initialize(ID3D11Device*) = 0;
	virtual bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	virtual void Render(ID3D11DeviceContext*) = 0;
	virtual int GetIndexCount() = 0;
	virtual void Shutdown() = 0;

	virtual ID3D11ShaderResourceView* GetTexture();
	virtual ID3D11ShaderResourceView** GetTextureArray();

	int GetShaderNumber();
	void SetShaderNumber(int);

private:
	int _shaderNumber;

};

#endif