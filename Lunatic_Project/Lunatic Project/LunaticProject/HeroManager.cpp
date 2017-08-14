#include "stdafx.h"
#include "HeroManager.h"


CHeroManager::CHeroManager(int nMeshes) : CGameObject(nMeshes)
{
	m_HeroSelect = EMPTY;
	m_Team = EMPTY;
	//m_pBabarian = NULL;
	//m_pHealer = NULL;
	//m_pSwordMan = NULL;
	hp = 0;
	attack = 0;
	speed = 1.f;
	normalspeed = speed;
	rootspeed = normalspeed / sqrt(2);


	// 공격 범위
	range = 0.f;


}


CHeroManager::~CHeroManager()
{
}
