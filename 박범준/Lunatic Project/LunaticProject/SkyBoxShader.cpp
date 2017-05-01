#include "stdafx.h"
#include "SkyBoxShader.h"


CSkyBoxShader::CSkyBoxShader()
{
}


CSkyBoxShader::~CSkyBoxShader()
{
}

void CSkyBoxShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	CSkyBox *pSkyBox = new CSkyBox(pd3dDevice);
	m_ppObjects[0] = pSkyBox;
	
}

void CSkyBoxShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::OnPrepareRender(pd3dDeviceContext);
	
	m_ppObjects[0]->Render(pd3dDeviceContext, pCamera);
}
