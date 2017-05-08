#pragma once
#include "GameObject.h"

class CCubeObject : public CGameObject
{
public:
	CCubeObject(int nMeshes = 1);
	~CCubeObject();

	virtual void Animate(float fTimeElapsed);
};

