#pragma once

#include "SordMan.h"
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

public:
	int m_CharSelect;
	int m_Team;

	CSordMan* m_pSordMan;
	CHealer* m_pHealer;
	CBabarian* m_pBabarian;

	CHeroManager(int nMeshes = 1);
	~CHeroManager();

	int GetHp() { return hp; }
	int GetAttack() { return attack; }
	float GetSpeed() { return speed; }
	float GetNormalSpeed() { return normalspeed; }
	float GetRootSpeed() { return rootspeed; }

	void SetHp(int _hp) { hp = _hp; }
	void SetAttack(float _attack) { attack = _attack; }
	void SetSpeed(float _speed) { speed = _speed; }
};

