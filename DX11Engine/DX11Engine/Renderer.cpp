#include "Renderer.h"

ID3D11ShaderResourceView* Renderer::GetTexture()
{
	return nullptr;
}

ID3D11ShaderResourceView** Renderer::GetTextureArray()
{
	return nullptr;
}

int Renderer::GetShaderNumber()
{
	return _shaderNumber;
}

void Renderer::SetShaderNumber(int shaderNumber)
{
	_shaderNumber = shaderNumber;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(ID3D11Device*, const WCHAR*, const WCHAR*)
{
	return false;
}
