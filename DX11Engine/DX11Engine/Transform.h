////////////////////////////////////////////////////////////////////////////////
// Filename: entity.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3dModule.h"
#include "Component.h"
#include "AlignedAllocationPolicy.h"

class Transform : public Component, public AlignedAllocationPolicy<16>
{
public:
	Transform();
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	XMMATRIX ConvertToLcoalMatrix(XMMATRIX);

private:
	XMMATRIX _matrix;

	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;
};

#endif