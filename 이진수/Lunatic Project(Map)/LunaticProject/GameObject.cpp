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


//////////////// Terrain
//
//CHeightMap::CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale)
//{
//	m_nWidth = nWidth;
//	m_nLength = nLength;
//	m_d3dxvScale = d3dxvScale;
//
//	BYTE *pHeightMapImage = new BYTE[m_nWidth * m_nLength];
//
//	//파일을 열고 읽는다. 높이 맵 이미지는 파일 헤더가 없는 RAW 이미지이다.
//	HANDLE hFile = ::CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
//	DWORD dwBytesRead;
//	::ReadFile(hFile, pHeightMapImage, (m_nWidth * m_nLength), &dwBytesRead, NULL);
//	::CloseHandle(hFile);
//
//	//이미지의 y-축과 지형의 z-축이 방향이 반대이므로 이미지를 상하대칭 시킨다.
//	m_pHeightMapImage = new BYTE[m_nWidth * m_nLength];
//	for (int y = 0; y < m_nLength; y++)
//	{
//		for (int x = 0; x < m_nWidth; x++)
//		{
//			m_pHeightMapImage[x + ((m_nLength - 1 - y)*m_nWidth)] = pHeightMapImage[x + (y*m_nWidth)];
//		}
//	}
//
//	if (pHeightMapImage) delete[] pHeightMapImage;
//}
//
//CHeightMap::~CHeightMap()
//{
//	if (m_pHeightMapImage) delete[] m_pHeightMapImage;
//	m_pHeightMapImage = NULL;
//}
//
//D3DXVECTOR3 CHeightMap::GetHeightMapNormal(int x, int z)
//{
//	//지형의 x-좌표와 z-좌표가 지형의 범위를 벗어나면 지형의 법선 벡터는 y-축 방향 벡터이다.
//	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength)) return(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
//
//	/*높이 맵에서 (x, z) 좌표의 픽셀 값과 인접한 두 개의 점 (x+1, z), (z, z+1)에 대한 픽셀 값을 사용하여 법선 벡터를 계산한다.*/
//	int nHeightMapIndex = x + (z * m_nWidth);
//	int xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
//	int zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -(signed)m_nWidth;
//	//(x, z), (x+1, z), (z, z+1)의 지형의 높이 값을 구한다.
//	float y1 = (float)m_pHeightMapImage[nHeightMapIndex] * m_d3dxvScale.y;
//	float y2 = (float)m_pHeightMapImage[nHeightMapIndex + xHeightMapAdd] * m_d3dxvScale.y;
//	float y3 = (float)m_pHeightMapImage[nHeightMapIndex + zHeightMapAdd] * m_d3dxvScale.y;
//
//	//vEdge1은 (0, y3, m_vScale.z) - (0, y1, 0) 벡터이다.
//	D3DXVECTOR3 vEdge1 = D3DXVECTOR3(0.0f, y3 - y1, m_d3dxvScale.z);
//	//vEdge2는 (m_vScale.x, y2, 0) - (0, y1, 0) 벡터이다.
//	D3DXVECTOR3 vEdge2 = D3DXVECTOR3(m_d3dxvScale.x, y2 - y1, 0.0f);
//	//법선 벡터는 vEdge1과 vEdge2의 외적을 정규화하면 된다.
//	D3DXVECTOR3 vNormal;
//	D3DXVec3Cross(&vNormal, &vEdge1, &vEdge2);
//	D3DXVec3Normalize(&vNormal, &vNormal);
//	return(vNormal);
//}
//
//float CHeightMap::GetHeight(float fx, float fz, bool bReverseQuad)
//{
//	//지형의 좌표 (fx, fz)에서 높이 맵의 좌표를 계산한다.
//	fx = fx / m_d3dxvScale.x;
//	fz = fz / m_d3dxvScale.z;
//	//높이 맵의 x-좌표와 z-좌표가 높이 맵의 범위를 벗어나면 지형의 높이는 0이다.
//	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength)) return(0.0f);
//	//높이 맵의 좌표의 정수 부분과 소수 부분을 계산한다.
//	int x = (int)fx;
//	int z = (int)fz;
//	float fxPercent = fx - x;
//	float fzPercent = fz - z;
//
//	float fTopLeft = m_pHeightMapImage[x + (z*m_nWidth)];
//	float fTopRight = m_pHeightMapImage[(x + 1) + (z*m_nWidth)];
//	float fBottomLeft = m_pHeightMapImage[x + ((z + 1)*m_nWidth)];
//	float fBottomRight = m_pHeightMapImage[(x + 1) + ((z + 1)*m_nWidth)];
//
//#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
//	if (bReverseQuad)
//	{
//		if (fxPercent <= fzPercent)
//			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
//		else
//			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
//	}
//	else
//	{
//		if (fxPercent < (1.0f - fzPercent))
//			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
//		else
//			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
//	}
//#endif
//
//	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
//	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
//	float fHeight = fTopHeight * (1 - fzPercent) + fBottomHeight * fzPercent;
//	//높이 맵의 픽셀 값을 지형의 높이로 바꾸어 반환한다.
//	return(fHeight * m_d3dxvScale.y);
//}
//
//CHeightMapTerrain::CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor)
//{
//	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
//	m_nWidth = nWidth;
//	m_nLength = nLength;
//
//	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
//	int cxQuadsPerBlock = nBlockWidth - 1;
//	int czQuadsPerBlock = nBlockLength - 1;
//
//	//d3dxvScale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
//	m_d3dxvScale = d3dxvScale;
//
//	//지형에 사용할 높이 맵을 생성한다.
//	m_pHeightMap = new CHeightMap(pFileName, nWidth, nLength, d3dxvScale);
//
//	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
//	int cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
//	int czBlocks = (m_nLength - 1) / czQuadsPerBlock;
//	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
//	m_nMeshes = cxBlocks * czBlocks;
//	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
//	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
//	m_ppMesh = new CHeightMapGridMesh*[m_nMeshes];
//
//	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
//	for (int z = 0, zStart = 0; z < czBlocks; z++)
//	{
//		for (int x = 0, xStart = 0; x < cxBlocks; x++)
//		{
//			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치이다.
//			xStart = x * (nBlockWidth - 1);
//			zStart = z * (nBlockLength - 1);
//			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
//			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, xStart, zStart, nBlockWidth, nBlockLength, d3dxvScale, d3dxColor, m_pHeightMap);
//			m_ppMesh[x + (z*cxBlocks)] = pHeightMapGridMesh;
//		}
//	}
//
//	D3DXMatrixIdentity(&m_d3dxmtxWorld);
//}
//
//CHeightMapTerrain::~CHeightMapTerrain(void)
//{
//	if (m_ppMesh)
//	{
//		for (int i = 0; i < m_nMeshes; i++) if (m_ppMesh[i]) delete m_ppMesh[i];
//		delete[] m_ppMesh;
//	}
//	if (m_pHeightMap) delete m_pHeightMap;
//}
//
//void CHeightMapTerrain::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera)
//{
//	//지형을 렌더링하는 것은 지형을 구성하는 각 격자 메쉬를 렌더링하는 것이다.
//	for (int i = 0; i < m_nMeshes; i++) m_ppMesh[i]->Render(pd3dImmediateDeviceContext);
//}