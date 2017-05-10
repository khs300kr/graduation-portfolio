#include "stdafx.h"
#include "SordMan.h"


CSordMan::CSordMan(int nMeshes) : CGameObject(nMeshes)
{
	hp = 100;
	attack = 50;
	speed = 0.3f;
	normalspeed = 0.3f;
	rootspeed = 0.3f / sqrt(2);
	
}


CSordMan::~CSordMan()
{

}
