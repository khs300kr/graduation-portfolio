#include "stdafx.h"
#include "HeroManager.h"


CHeroManager::CHeroManager(int nMeshes) : CGameObject(nMeshes)
{
	m_HeroSelect = Healer;
	m_Team = A_TEAM;

	hp = 100;
	attack = 50;
	speed = 0.3f;
	normalspeed = 0.3f;
	rootspeed = 0.3f / sqrt(2);


	
}


CHeroManager::~CHeroManager()
{
}
