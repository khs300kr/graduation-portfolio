#pragma once
#include "Shader.h"

class CCharacterShader : public CIlluminatedShader
{
public:
	CCharacterShader(int nObjects = 1);
	~CCharacterShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};

