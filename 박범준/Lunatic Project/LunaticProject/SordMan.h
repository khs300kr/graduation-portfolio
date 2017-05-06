#pragma once

#include "GameObject.h"
#include "Camera.h"


class CSordMan : public CGameObject
{
private:
	int hp;
	float attack;
	float speed;
	float normalspeed;
	float rootspeed;




public:
	CSordMan(int nMeshes = 1);
	~CSordMan();


	
	int GetHp() { return hp; }
	int GetAttack() { return attack; }
	float GetSpeed() { return speed; }
	float GetNormalSpeed() { return normalspeed; }
	float GetRootSpeed() { return rootspeed; }

	void SetHp(int _hp) { hp = _hp; }
	void SetAttack(float _attack) { attack = _attack; }
	void SetSpeed(float _speed) { speed = _speed; }

};

