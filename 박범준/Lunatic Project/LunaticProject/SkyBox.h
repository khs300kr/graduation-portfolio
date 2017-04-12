#pragma once
#include "GameObject.h"
#include "SkyBoxMesh.h"
#include "Shader.h"

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D11Device *pd3dDevice);
	virtual ~CSkyBox();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
	virtual void Animate(float fTimeElapsed);
};