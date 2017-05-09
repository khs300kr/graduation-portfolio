#include "stdafx.h"
#include "SkyBox.h"



CSkyBox::CSkyBox(ID3D11Device *pd3dDevice) : CGameObject(1)
{
	CSkyBoxMesh *pSkyBoxMesh = new CSkyBoxMesh(pd3dDevice, 2000.0f, 2000.0f, 2000.0f);
	SetMesh(pSkyBoxMesh, 0);
	CameraPos = D3DXVECTOR3(0, 40, -50);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	// 스카이 박스 객체의 위치를 카메라의 위치로 변경
	/*D3DXVECTOR3 d3dxvCameraPos = pCamera->GetPosition();
	SetPosition(d3dxvCameraPos.x, d3dxvCameraPos.y, d3dxvCameraPos.z);*/
	
	SetPosition(CameraPos.x, CameraPos.y, CameraPos.z);

	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);

	// 스카이 박스 메쉬(6개의 사각형)를 렌더링
	if (m_ppMeshes && m_ppMeshes[0]) m_ppMeshes[0]->Render(pd3dDeviceContext);
}

void CSkyBox::Animate(float fTimeElapsed)
{
	m_d3dxmtxWorld = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
}