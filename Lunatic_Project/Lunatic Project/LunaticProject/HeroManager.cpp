#include "stdafx.h"
#include "HeroManager.h"


CHeroManager::CHeroManager(int nMeshes) : CGameObject(nMeshes)
{
	m_HeroSelect = EMPTY;
	m_Team = EMPTY;
	//m_pBabarian = NULL;
	//m_pHealer = NULL;
	//m_pSwordMan = NULL;
	hp = 100;
	attack = 50;
	speed = 0.5f;
	normalspeed = speed;
	rootspeed = normalspeed / sqrt(2);


	
}


CHeroManager::~CHeroManager()
{
}
