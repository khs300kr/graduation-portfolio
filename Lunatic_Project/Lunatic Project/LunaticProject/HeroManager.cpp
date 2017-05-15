#include "stdafx.h"
#include "HeroManager.h"


CHeroManager::CHeroManager(int nMeshes) : CGameObject(nMeshes)
{
	m_HeroSelect = Empty;
	m_Team = Empty;
	m_pBabarian = NULL;
	m_pHealer = NULL;
	m_pSordMan = NULL;
	hp = 100;
	attack = 50;
	speed = 0.3f;
	normalspeed = speed;
	rootspeed = normalspeed / sqrt(2);


	
}


CHeroManager::~CHeroManager()
{
}
