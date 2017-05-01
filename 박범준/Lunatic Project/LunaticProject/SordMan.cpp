#include "stdafx.h"
#include "SordMan.h"


CSordMan::CSordMan(int nMeshes) : CGameObject(nMeshes)
{
	hp = 100;
	attack = 50;
	speed = 0.15f;
	normalspeed = 0.15f;
	rootspeed = 0.15f / sqrt(2);
	
}


CSordMan::~CSordMan()
{

}
