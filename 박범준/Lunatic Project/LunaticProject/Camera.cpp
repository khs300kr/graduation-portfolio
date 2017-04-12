#include "stdafx.h"
#include "Player.h"
#include "Camera.h"



CCamera::CCamera(CCamera *pCamera)
{
	if (pCamera)
	{
		// 카메라가 이미 있으면 기존 카메라의 정보를 새로운 카메라에 복사 
		m_d3dxvPosition = pCamera->GetPosition();
		m_d3dxvRight = pCamera->GetRightVector();
		m_d3dxvLook = pCamera->GetLookVector();
		m_d3dxvUp = pCamera->GetUpVector();
		m_fPitch = pCamera->GetPitch();
		m_fRoll = pCamera->GetRoll();
		m_fYaw = pCamera->GetYaw();
		m_d3dxmtxView = pCamera->GetViewMatrix();
		m_d3dxmtxProjection = pCamera->GetProjectionMatrix();
		m_d3dViewport = pCamera->GetViewport();
		m_d3dxvLookAtWorld = pCamera->GetLookAtPosition();
		m_d3dxvOffset = pCamera->GetOffset();
		m_fTimeLag = pCamera->GetTimeLag();
		m_pPlayer = pCamera->GetPlayer();
		m_pd3dcbCamera = pCamera->GetCameraConstantBuffer();
		if (m_pd3dcbCamera) m_pd3dcbCamera->AddRef();
	}
	else
	{
		// 카메라가 없으면 기본 정보를 설정 
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = 0.0f;
		m_fTimeLag = 0.0f;
		m_d3dxvLookAtWorld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nMode = 0x00;
		m_pPlayer = NULL;
		D3DXMatrixIdentity(&m_d3dxmtxView);
		D3DXMatrixIdentity(&m_d3dxmtxProjection);
		m_pd3dcbCamera = NULL;
	}
}

CCamera::~CCamera()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();
}

// Direct3D 디바이스의 뷰포트를 설정하기 위한 함수이다.
void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

// 카메라 변환 행렬을 생성
void CCamera::GenerateViewMatrix()
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &m_d3dxvPosition, &m_d3dxvLookAtWorld, &m_d3dxvUp);
}

void CCamera::RegenerateViewMatrix()
{
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);

	m_d3dxmtxView._11 = m_d3dxvRight.x;
	m_d3dxmtxView._12 = m_d3dxvUp.x;
	m_d3dxmtxView._13 = m_d3dxvLook.x;
	m_d3dxmtxView._21 = m_d3dxvRight.y;
	m_d3dxmtxView._22 = m_d3dxvUp.y;
	m_d3dxmtxView._23 = m_d3dxvLook.y;
	m_d3dxmtxView._31 = m_d3dxvRight.z;
	m_d3dxmtxView._32 = m_d3dxvUp.z;
	m_d3dxmtxView._33 = m_d3dxvLook.z;
	m_d3dxmtxView._41 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvRight);
	m_d3dxmtxView._42 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvUp);
	m_d3dxmtxView._43 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvLook);

	// 카메라의 위치, 방향이 바뀌면 절두체 평면 역시 재 계산
	CalculateFrustumPlanes();
}

// 투영 변환 행렬을 생성
void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

// 카메라 변환 & 투영 변환 행렬을 쉐이더로 전달하기 위한
// 구조체에 대한 상수 버퍼를 생성하는 함수
void CCamera::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

// 카메라 변환 행렬 & 투영 변환 행렬을 쉐이더로 전달하기 위해
// 구조체에 대한 상수 버퍼를 갱신 후 디바이스 컨텍스트와 연결
void CCamera::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	/*상수 버퍼의 메모리 주소를 가져와서 카메라 변환 행렬과 투영 변환 행렬을 복사한다. 쉐이더에서 행렬의 행과 열이 바뀌는 것에 주의하라.*/
	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	//상수 버퍼를 슬롯(VS_SLOT_CAMERA)에 설정한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_CAMERA, 1, &m_pd3dcbCamera);
}


//
// CSpaceShipCamera
//////////////////////////////////////////////////////////////////////////////////////////
CSpaceShipCamera::CSpaceShipCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_nMode = SPACESHIP_CAMERA;
}

void CSpaceShipCamera::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (m_pPlayer && (x != 0.0f))
	{
		//플레이어의 로컬 x-축에 대한 x 각도의 회전 행렬을 계산한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetRightVector(), (float)D3DXToRadian(x));
		//카메라의 로컬 x-축, y-축, z-축을 회전한다.
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
		/*카메라의 위치 벡터에서 플레이어의 위치 벡터를 뺀다. 결과는 플레이어 위치를 기준으로 한 카메라의 위치 벡터이다.*/
		m_d3dxvPosition -= m_pPlayer->GetPosition();
		//플레이어의 위치를 중심으로 카메라의 위치 벡터(플레이어를 기준으로 한)를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
		//회전시킨 카메라의 위치 벡터에 플레이어의 위치를 더한다.
		m_d3dxvPosition += m_pPlayer->GetPosition();
	}
	if (m_pPlayer && (y != 0.0f))
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetUpVector(), (float)D3DXToRadian(y));
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
		m_d3dxvPosition -= m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
		m_d3dxvPosition += m_pPlayer->GetPosition();
	}
	if (m_pPlayer && (z != 0.0f))
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetLookVector(), (float)D3DXToRadian(z));
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
		m_d3dxvPosition -= m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
		m_d3dxvPosition += m_pPlayer->GetPosition();
	}
}


//
// CFirstPersonCamera
//////////////////////////////////////////////////////////////////////////////////////////
CFirstPersonCamera::CFirstPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_nMode = FIRST_PERSON_CAMERA;
	if (pCamera)
	{
		/*1인칭 카메라로 변경하기 이전의 카메라가 스페이스-쉽 카메라이면 카메라의 Up 벡터를 월드좌표의 y-축이 되도록 한다. 이것은 스페이스-쉽 카메라의 로컬 y-축 벡터가 어떤 방향이든지 1인칭 카메라(대부분 사람인 경우)의 로컬 y-축 벡터가 월드좌표의 y-축이 되도록 즉, 똑바로 서있는 형태로 설정한다는 의미이다. 그리고 로컬 x-축 벡터와 로컬 z-축 벡터의 y-좌표가 0.0f가 되도록 한다. 이것은 <그림 8>과 같이 로컬 x-축 벡터와 로컬 z-축 벡터를 xz-평면(지면)으로 투영하는 것을 의미한다. 즉, 1인칭 카메라의 로컬 x-축 벡터와 로컬 z-축 벡터는 xz-평면에 평행하다.*/
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;
			D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
			D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		}
	}
}
void CFirstPersonCamera::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (x != 0.0f)
	{
		// 카메라의 로컬 x-축을 기준으로 회전하는 행렬을 생성한다. 고개를 끄떡이는 동작이다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));
		// 카메라의 로컬 x-축, y-축, z-축을 회전한다.
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}
	if (m_pPlayer && (y != 0.0f))
	{
		// 플레이어의 로컬 y-축을 기준으로 회전하는 행렬을 생성한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetUpVector(), (float)D3DXToRadian(y));
		// 카메라의 로컬 x-축, y-축, z-축을 회전한다.
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}
	if (m_pPlayer && (z != 0.0f))
	{
		// 플레이어의 로컬 z-축을 기준으로 회전하는 행렬을 생성한다.
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetLookVector(), (float)D3DXToRadian(z));
		// 카메라의 위치 벡터를 플레이어 좌표계로 표현한다(오프셋 벡터).
		m_d3dxvPosition -= m_pPlayer->GetPosition();
		// 오프셋 벡터 벡터를 회전한다.
		D3DXVec3TransformCoord(&m_d3dxvPosition, &m_d3dxvPosition, &mtxRotate);
		// 회전한 카메라의 위치를 월드 좌표계로 표현한다.
		m_d3dxvPosition += m_pPlayer->GetPosition();
		//카메라의 로컬 x-축, y-축, z-축을 회전한다.
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
	}
}


//
// CThirdPersonCamera
//////////////////////////////////////////////////////////////////////////////////////////
CThirdPersonCamera::CThirdPersonCamera(CCamera *pCamera) : CCamera(pCamera)
{
	m_nMode = THIRD_PERSON_CAMERA;
	if (pCamera)
	{
		// 3인칭 카메라로 변경하기 이전의 카메라가 스페이스-쉽 카메라이면 카메라의 Up 벡터를 월드좌표의 y-축이 되도록 한다.
		// 이것은 스페이스-쉽 카메라의 로컬 y-축 벡터가 어떤 방향이든지 3인칭 카메라(대부분 사람인 경우)의 로컬 y-축 벡터가 
		// 월드좌표의 y-축이 되도록 즉, 똑바로 서있는 형태로 설정한다는 의미이다. 그리고 로컬 x-축 벡터와 로컬 z-축 벡터의 
		// y-좌표가 0.0f가 되도록 한다. 이것은 로컬 x-축 벡터와 로컬 z-축 벡터를 xz-평면(지면)으로 투영하는 것을 의미한다.
		// 즉, 3인칭 카메라의 로컬 x-축 벡터와 로컬 z-축 벡터는 xz-평면에 평행하다.
		if (pCamera->GetMode() == SPACESHIP_CAMERA)
		{
			m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_d3dxvRight.y = 0.0f;
			m_d3dxvLook.y = 0.0f;
			D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
			D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		}
	}
}

void CThirdPersonCamera::Update(float fTimeElapsed)
{
	// 플레이어의 회전에 따라 3인칭 카메라도 회전해야 한다.
	if (m_pPlayer)
	{
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity(&mtxRotate);
	D3DXVECTOR3 d3dxvRight = m_pPlayer->GetRightVector();
	D3DXVECTOR3 d3dxvUp = m_pPlayer->GetUpVector();
	D3DXVECTOR3 d3dxvLook = m_pPlayer->GetLookVector();
	//플레이어의 로컬 x-축, y-축, z-축 벡터로부터 회전 행렬을 생성한다.
	mtxRotate._11 = d3dxvRight.x; mtxRotate._21 = d3dxvUp.x; mtxRotate._31 = d3dxvLook.x;
	mtxRotate._12 = d3dxvRight.y; mtxRotate._22 = d3dxvUp.y; mtxRotate._32 = d3dxvLook.y;
	mtxRotate._13 = d3dxvRight.z; mtxRotate._23 = d3dxvUp.z; mtxRotate._33 = d3dxvLook.z;

	D3DXVECTOR3 d3dxvOffset;
	D3DXVec3TransformCoord(&d3dxvOffset, &m_d3dxvOffset, &mtxRotate);
	// 회전한 카메라의 위치는 플레이어의 위치에 회전한 카메라 오프셋 벡터를 더한 것이다.
	D3DXVECTOR3 d3dxvPosition = m_pPlayer->GetPosition() +d3dxvOffset;
	// 현재의 카메라의 위치에서 회전한 카메라의 위치까지의 벡터이다.
	D3DXVECTOR3 d3dxvDirection = d3dxvPosition - m_d3dxvPosition;
	float fLength = D3DXVec3Length(&d3dxvDirection);
	D3DXVec3Normalize(&d3dxvDirection, &d3dxvDirection);
	// 3인칭 카메라의 래그(Lag)는 플레이어가 회전하더라도 
	// 카메라가 동시에 따라서 회전하지 않고 약간의 시차를 두고 회전하는 효과를 구현하기 위한 것
	// m_fTimeLag가 1보다 크면 fTimeLagScale이 작아지고 실제 회전이 적게 일어날 것
	float fTimeLagScale = (m_fTimeLag) ? fTimeElapsed * (1.0f / m_fTimeLag) : 1.0f;
	float fDistance = fLength * fTimeLagScale;
	if (fDistance > fLength) fDistance = fLength;
	if (fLength < 0.01f) fDistance = fLength;
	if (fDistance > 0)
	{
		m_d3dxvPosition += d3dxvDirection * fDistance;
		SetLookAt(GetLookAtPosition());
	}
}
}

void CThirdPersonCamera::SetLookAt(D3DXVECTOR3& d3dxvLookAt)
{
	D3DXMATRIX mtxLookAt;
	D3DXMatrixLookAtLH(&mtxLookAt, &m_d3dxvPosition, &d3dxvLookAt, &m_pPlayer->GetUpVector());
	m_d3dxvRight = D3DXVECTOR3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_d3dxvUp = D3DXVECTOR3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_d3dxvLook = D3DXVECTOR3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}


//
// FrustumCulling Fuction
//////////////////////////////////////////////////////////////////////////////////////////
void CCamera::CalculateFrustumPlanes()
{
	// 카메라 변환 + 원근 투영 행렬을 곱한 행렬(월드 행렬)을 이용해 절두체 평면을 정의
	D3DXMATRIX mtxViewProject = m_d3dxmtxView * m_d3dxmtxProjection;

	// 절두체의 왼쪽 평면
	m_d3dxFrustumPlanes[0].a = -(mtxViewProject._14 + mtxViewProject._11);
	m_d3dxFrustumPlanes[0].b = -(mtxViewProject._24 + mtxViewProject._21);
	m_d3dxFrustumPlanes[0].c = -(mtxViewProject._34 + mtxViewProject._31);
	m_d3dxFrustumPlanes[0].d = -(mtxViewProject._44 + mtxViewProject._41);

	// 절두체의 오른쪽 평면
	m_d3dxFrustumPlanes[1].a = -(mtxViewProject._14 - mtxViewProject._11);
	m_d3dxFrustumPlanes[1].b = -(mtxViewProject._24 - mtxViewProject._21);
	m_d3dxFrustumPlanes[1].c = -(mtxViewProject._34 - mtxViewProject._31);
	m_d3dxFrustumPlanes[1].d = -(mtxViewProject._44 - mtxViewProject._41);

	// 절두체의 위쪽 평면
	m_d3dxFrustumPlanes[2].a = -(mtxViewProject._14 - mtxViewProject._12);
	m_d3dxFrustumPlanes[2].b = -(mtxViewProject._24 - mtxViewProject._22);
	m_d3dxFrustumPlanes[2].c = -(mtxViewProject._34 - mtxViewProject._32);
	m_d3dxFrustumPlanes[2].d = -(mtxViewProject._44 - mtxViewProject._42);

	// 절두체의 아래쪽 평면
	m_d3dxFrustumPlanes[3].a = -(mtxViewProject._14 + mtxViewProject._12);
	m_d3dxFrustumPlanes[3].b = -(mtxViewProject._24 + mtxViewProject._22);
	m_d3dxFrustumPlanes[3].c = -(mtxViewProject._34 + mtxViewProject._32);
	m_d3dxFrustumPlanes[3].d = -(mtxViewProject._44 + mtxViewProject._42);

	// 절두체의 근평면
	m_d3dxFrustumPlanes[4].a = -(mtxViewProject._13);
	m_d3dxFrustumPlanes[4].b = -(mtxViewProject._23);
	m_d3dxFrustumPlanes[4].c = -(mtxViewProject._33);
	m_d3dxFrustumPlanes[4].d = -(mtxViewProject._43);

	// 절두체의 원평면
	m_d3dxFrustumPlanes[5].a = -(mtxViewProject._14 - mtxViewProject._13);
	m_d3dxFrustumPlanes[5].b = -(mtxViewProject._24 - mtxViewProject._23);
	m_d3dxFrustumPlanes[5].c = -(mtxViewProject._34 - mtxViewProject._33);
	m_d3dxFrustumPlanes[5].d = -(mtxViewProject._44 - mtxViewProject._43);

	for (int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&m_d3dxFrustumPlanes[i], &m_d3dxFrustumPlanes[i]);
}


bool CCamera::IsInFrustum(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum)
{
	D3DXVECTOR3 d3dxvNearPoint, d3dxvFarPoint, d3dxvNormal;
	for (int i = 0; i < 6; i++)
	{
		// 절두체의 각 평면에 대하여 바운딩 박스의 근접점을 계산한다.
		// 근접점의 x, y, z 좌표는 법선 벡터의 각 요소가 음수이면 바운딩 박스의 
		// 최대점의 좌표가 되고 그렇지 않으면 바운딩 박스의 최소점의 좌표가 된다.
		d3dxvNormal = D3DXVECTOR3(m_d3dxFrustumPlanes[i].a, m_d3dxFrustumPlanes[i].b, m_d3dxFrustumPlanes[i].c);
		if (d3dxvNormal.x >= 0.0f)
		{
			if (d3dxvNormal.y >= 0.0f)
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					// 법선 벡터의 x, y, z 좌표의 부호가 모두 양수이므로
					// 근접점은 바운딩 박스의 최소점이다.
					d3dxvNearPoint.x = d3dxvMinimum.x;
					d3dxvNearPoint.y = d3dxvMinimum.y;
					d3dxvNearPoint.z = d3dxvMinimum.z;
				}
				else
				{
					// 법선 벡터의 x, y 좌표의 부호가 모두 양수이므로 
					// 근접점의 x, y 좌표는 바운딩 박스의 최소점의 x, y 좌표이고 
					// 법선 벡터의 z 좌표가 음수이므로 근접점의 z 좌표는 바운딩 박스의 
					// 최대점의 z 좌표이다.
					d3dxvNearPoint.x = d3dxvMinimum.x;
					d3dxvNearPoint.y = d3dxvMinimum.y;
					d3dxvNearPoint.z = d3dxvMaximum.z;
				}
			}
			else
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					// 법선 벡터의 x, z 좌표의 부호가 모두 양수이므로 
					// 근접점의 x, z 좌표는 바운딩 박스의 최소점의 x, z 좌표이고 
					// 법선벡터의 y 좌표가 음수이므로 근접점의 y 좌표는 바운딩 박스의
					// 최대점의 y 좌표이다.
					d3dxvNearPoint.x = d3dxvMinimum.x;
					d3dxvNearPoint.y = d3dxvMaximum.y;
					d3dxvNearPoint.z = d3dxvMinimum.z;
				}
				else
				{
					// 법선 벡터의 y, z 좌표의 부호가 모두 음수이므로 
					// 근접점의 y, z 좌표는 바운딩 박스의 최대점의 y, z 좌표이고
					// 법선 벡터의 x 좌표가 양수이므로 근접점의 x 좌표는
					// 바운딩 박스의 최소점의 x 좌표이다.
					d3dxvNearPoint.x = d3dxvMinimum.x;
					d3dxvNearPoint.y = d3dxvMaximum.y; 
					d3dxvNearPoint.z = d3dxvMaximum.z;
				}
			}
		}
		else
		{
			if (d3dxvNormal.y >= 0.0f)
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					// 법선 벡터의 y, z 좌표의 부호가 모두 양수이므로 
					// 근접점의 y, z 좌표는 바운딩 박스의 최소점의 y, z 좌표이고 
					// 법선벡터의 x 좌표가 음수이므로 근접점의 x 좌표는 
					// 바운딩 박스의 최대점의 x 좌표이다.
					d3dxvNearPoint.x = d3dxvMaximum.x;
					d3dxvNearPoint.y = d3dxvMinimum.y; 
					d3dxvNearPoint.z = d3dxvMinimum.z;
				}
				else
				{
					// 법선 벡터의 x, z 좌표의 부호가 모두 음수이므로 
					// 근접점의 x, z 좌표는 바운딩 박스의 최대점의 x, z 좌표이고 
					// 법선 벡터의 y 좌표가 양수이므로 근접점의 y 좌표는
					// 바운딩 박스의 최소점의 y 좌표이다.
					d3dxvNearPoint.x = d3dxvMaximum.x; 
					d3dxvNearPoint.y = d3dxvMinimum.y;
					d3dxvNearPoint.z = d3dxvMaximum.z;
				}
			}
			else
			{
				if (d3dxvNormal.z >= 0.0f)
				{
					// 법선 벡터의 x, y 좌표의 부호가 모두 음수이므로 
					// 근접점의 x, y 좌표는 바운딩 박스의 최대점의 x, y 좌표이고 
					// 법선 벡터의 z 좌표가 양수이므로 근접점의 z 좌표는 바운딩 박스의
					// 최소점의 z 좌표이다.
					d3dxvNearPoint.x = d3dxvMaximum.x; d3dxvNearPoint.y = d3dxvMaximum.y; d3dxvNearPoint.z = d3dxvMinimum.z;
				}
				else
				{
					// 법선 벡터의 x, y, z 좌표의 부호가 모두 음수이므로 
					// 근접점은 바운딩 박스의 최대점이다.
					d3dxvNearPoint.x = d3dxvMaximum.x; d3dxvNearPoint.y = d3dxvMaximum.y; d3dxvNearPoint.z = d3dxvMaximum.z;
				}
			}
		}
		
		// 근접점이 절두체 평면 중 하나의 평면의 바깥(앞)쪽에 있으면 
		// 근접점은 절두체에 포함되지 않는다. 근접점이 어떤 평면의 바깥(앞)쪽에 있으면
		// 근접점을 평면의 방정식에 대입하면 부호가 양수가 된다. 각 평면의 방정식에 근접점을
		// 대입하는 것은 근접점 (x, y, z)과 평면의 법선벡터 (a, b, c)의 내적에 
		// 원점에서 평면까지의 거리를 더한 것과 같다.		
		if ((D3DXVec3Dot(&d3dxvNormal, &d3dxvNearPoint) + m_d3dxFrustumPlanes[i].d) > 0.0f)
			return false;
	}
	return true;
}

bool CCamera::IsInFrustum(AABB *pAABB)
{
	return IsInFrustum(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum);
}
