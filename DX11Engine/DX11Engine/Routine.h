////////////////////////////////////////////////////////////////////////////////
// Filename: routine.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ROUTINE_H_
#define _ROUTINE_H_

/// <summary>
/// GameObject, light ��� �پ��� Ŭ������ �Ļ� ����
/// </summary>
class Routine
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;

	virtual void OnDestroy() = 0;
};

#endif