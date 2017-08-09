#pragma once

#include "SwordMan.h"
#include "Healer.h"
#include "Babarian.h"

class CHeroManager : public CGameObject
{
private:
	int hp;
	float attack;
	float speed;
	float normalspeed;
	float rootspeed;
	float range; // 공격범위

public:
	int m_HeroSelect;
	int m_Team;

	bool bHeroAttack = false;
	bool bHeroRun = false;
	bool bHeroQ = false;
	bool bHeroW = false;
	bool bHeroE = false;
	bool bHeroR = false;
	bool bHeroHit = false;
	bool bHeroStun = false;
	bool bHeroDie = false;

	//CSwordMan* m_pSwordMan;
	//CHealer* m_pHealer;
	//CBabarian* m_pBabarian;

	CHeroManager(int nMeshes = 1);
	~CHeroManager();

	int GetHp() { return hp; }
	int GetAttack() { return attack; }
	float GetSpeed() { return speed; }
	float GetNormalSpeed() { return normalspeed; }
	float GetRootSpeed() { return rootspeed; }
	float GetRange() { return range; }

	void SetHp(int _hp) { hp = _hp; }
	void SetAttack(float _attack) { attack = _attack; }
	void SetSpeed(float _speed) { speed = _speed; }
	void SetRange(float _range) { range = _range; }
};

