#include "stdafx.h"
#include "HeroManager.h"


CHeroManager::CHeroManager(int nMeshes) : CGameObject(nMeshes)
{
	m_HeroSelect = SordMan;
	m_Team = B_TEAM;

	hp = 100;
	attack = 50;
	speed = 0.3f;
	normalspeed = speed;
	rootspeed = normalspeed / sqrt(2);


	
}


CHeroManager::~CHeroManager()
{
}
