#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"


//
// CMaterial Class
//////////////////////////////////////////////////////////////////////////////////////////
CMaterial::CMaterial()
{
	m_nReferences = 0;
}

CMaterial::~CMaterial()
{
}


//
// CTexture Class
//////////////////////////////////////////////////////////////////////////////////////////
CTexture::CTexture(int nTextures, int nSamplers, int nTextureStartSlot, int nSamplerStartSlot)
{
	m_nReferences = 0;

	m_nTextures = nTextures;
	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; i++) m_ppd3dsrvTextures[i] = NULL;
	m_nTextureStartSlot = nTextureStartSlot;
	m_nSamplers = nSamplers;
	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nSamplers];
	for (int i = 0; i < m_nSamplers; i++) m_ppd3dSamplerStates[i] = NULL;
	m_nSamplerStartSlot = nSamplerStartSlot;
}

CTexture::~CTexture()
{
	for (int i = 0; i < m_nTextures; i++) if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->Release();
	for (int i = 0; i < m_nSamplers; i++) if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->Release();
	if (m_ppd3dsrvTextures) delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) delete[] m_ppd3dSamplerStates;
}

void CTexture::SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	if (pd3dsrvTexture) pd3dsrvTexture->AddRef();
}

void CTexture::SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState)
{
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	if (pd3dSamplerState) pd3dSamplerState->AddRef();
}

void CTexture::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->PSSetShaderResources(m_nTextureStartSlot, m_nTextures, m_ppd3dsrvTextures);
	pd3dDeviceContext->PSSetSamplers(m_nSamplerStartSlot, m_nSamplers, m_ppd3dSamplerStates);
}

void CTexture::UpdateTextureShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex, int nSlot)
{
	pd3dDeviceContext->PSSetShaderResources(nSlot, 1, &m_ppd3dsrvTextures[nIndex]);
}

void CTexture::UpdateSamplerShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex, int nSlot)
{
	pd3dDeviceContext->PSSetSamplers(nSlot, 1, &m_ppd3dSamplerStates[nIndex]);
}



//
// CGameObject Class
//////////////////////////////////////////////////////////////////////////////////////////
CGameObject::CGameObject(int nMeshes)
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	D3DXMatrixIdentity(&m_d3dxmtxRotate);
	D3DXMatrixIdentity(&m_d3dxmtxScale);
	D3DXMatrixIdentity(&m_d3dxmtxTranlate); 
	m_d3dxvDirection = D3DXVECTOR3(0, 0, 0);
	m_fRotationAngle = 0.0f;
	m_nMeshes = nMeshes;
	m_ppMeshes = NULL;
	m_bcMeshBoundingCube = AABB();
	m_bActive = true;
	m_nReferences = 0;
	m_pMaterial = NULL;
	m_pTexture = NULL;
	m_pCollider = NULL;

	if (m_nMeshes > 0) m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)m_ppMeshes[i] = NULL;
}

CGameObject::~CGameObject()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture) m_pTexture->Release();
}

void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}

void CGameObject::SetMesh(CMesh *pMesh, int nIndex)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}

	if (pMesh)
	{
		AABB bcBoundingCube = pMesh->GetBoundingCube();
		m_bcMeshBoundingCube.Union(&bcBoundingCube);
	}
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::Animate(float fTimeElapsed)
{
	// 애니메이션이 있는 메쉬의 사이즈는 여기서 조정해준다.
	if (GetMesh(0)->GetFBXModelSize() != 0.0f || GetMesh(0)->GetFBXModelSize() != 1.0f)
	{
		D3DXMATRIX mtxScale;
		D3DXMatrixScaling(&mtxScale, GetMesh(0)->GetFBXModelSize(), GetMesh(0)->GetFBXModelSize(), GetMesh(0)->GetFBXModelSize());
		m_d3dxmtxScale = mtxScale;
	}

	m_d3dxmtxWorld = m_d3dxmtxScale * m_d3dxmtxRotate * m_d3dxmtxTranlate;
}


void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
	// 재질이 있을 경우 상수 버퍼에 연결
	if (m_pMaterial) CIlluminatedShader::UpdateShaderVariable(pd3dDeviceContext, &m_pMaterial->m_Material);
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pd3dDeviceContext);	
	
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i])
			{
				bool bIsVisible = true;
				if (pCamera)
				{
					AABB bcBoundingCube = m_ppMeshes[i]->GetBoundingCube();
					bcBoundingCube.Update(&m_d3dxmtxWorld);
					bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
				}
				if (bIsVisible)
					m_ppMeshes[i]->Render(pd3dDeviceContext);
			}
		}
	}
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxTranlate._41 = x;
	m_d3dxmtxTranlate._42 = y;
	m_d3dxmtxTranlate._43 = z;
	//m_d3dxmtxWorld._41 = x;
	//m_d3dxmtxWorld._42 = y;
	//m_d3dxmtxWorld._43 = z;
}

void CGameObject::Move(D3DXVECTOR3 Position, DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		D3DXVECTOR3 d3dxvShift = Position;
		// 화살표 키 [↑], [↓]를 누르면 로컬 z축 방향으로 이동
		if (dwDirection & DIR_FRONT) d3dxvShift.z = -d3dxvShift.z * fDistance;
		if (dwDirection & DIR_BACK) d3dxvShift.z = d3dxvShift.z  * fDistance;
		// 화살표 키 [→], [←]를 누르면 로컬 x축 방향으로 이동
		if (dwDirection & DIR_RIGHT) d3dxvShift.x = d3dxvShift.x * fDistance;
		if (dwDirection & DIR_LEFT) d3dxvShift.x = -d3dxvShift.x * fDistance;
		// [Page Up], [Page Down]을 누르면 로컬 y축 방향으로 이동
		//if (dwDirection & DIR_UP) d3dxvShift += m_d3dxvUp * fDistance;
		//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;

		//SetPosition(d3dxvShift);
		// 플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.
		//Move(d3dxvShift, bUpdateVelocity);
	}
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxTranlate._41 = d3dxvPosition.x;
	m_d3dxmtxTranlate._42 = d3dxvPosition.y;
	m_d3dxmtxTranlate._43 = d3dxvPosition.z;
}

D3DXVECTOR3 CGameObject::GetPosition()
{
	return D3DXVECTOR3(m_d3dxmtxTranlate._41, m_d3dxmtxTranlate._42, m_d3dxmtxTranlate._43);
}

D3DXVECTOR3 CGameObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return d3dxvLookAt;
}

D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return d3dxvUp;
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return d3dxvRight;
}

void CGameObject::MoveRight(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxRotate = mtxRotate;// *m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxRotate = mtxRotate;// *m_d3dxmtxWorld;
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	OnPrepareRender();

	bool bIsVisible = false;
	if (m_bActive)
	{
		AABB bcBoundingCube = m_bcMeshBoundingCube;
		// 객체의 메쉬의 바운딩박스(모델 좌표계)를 객체의 월드 변환 행렬로 변환,
		// 이후 새로운 바운딩 박스를 계산
		bcBoundingCube.Update(&m_d3dxmtxWorld);
		// 바운딩 박스(월드 좌표계)가 카메라의 절두체에 포함되는 가를 검사한다. 
		if (pCamera) bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
	}
	return bIsVisible;
}

void CGameObject::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

