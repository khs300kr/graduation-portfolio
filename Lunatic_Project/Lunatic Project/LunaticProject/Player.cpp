#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer(int nMeshes) : CGameObject(nMeshes)
{
	m_pCamera = NULL;

	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_d3dxvVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;

	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;

	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
}

void CPlayer::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
}

void CPlayer::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	// 플레이어의 현재 카메라의 UpdateShaderVariables() 멤버 함수를 호출한다.
	if (m_pCamera) m_pCamera->UpdateShaderVariables(pd3dDeviceContext);
}

// 플레이어의 위치를 변경
// 플레이어의 위치는 기본적으로 사용자가 플레이어를 이동하기 위한 키보드를 누를 때 변경된다.
// 플레이어의 이동 방향(dwDirection)에 따라 플레이어를 fDistance 만큼 이동한다.
void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		D3DXVECTOR3 d3dxvShift = D3DXVECTOR3(0, 0, 0);
		// 화살표 키 [↑], [↓]를 누르면 로컬 z축 방향으로 이동
		if (dwDirection & DIR_FRONT) d3dxvShift -= m_d3dxvLook * fDistance;
		if (dwDirection & DIR_BACK) d3dxvShift += m_d3dxvLook * fDistance;
		// 화살표 키 [→], [←]를 누르면 로컬 x축 방향으로 이동
		if (dwDirection & DIR_RIGHT) d3dxvShift += m_d3dxvRight * fDistance;
		if (dwDirection & DIR_LEFT) d3dxvShift -= m_d3dxvRight * fDistance;
		// [Page Up], [Page Down]을 누르면 로컬 y축 방향으로 이동
		//if (dwDirection & DIR_UP) d3dxvShift += m_d3dxvUp * fDistance;
		//if (dwDirection & DIR_DOWN) d3dxvShift -= m_d3dxvUp * fDistance;

		// 플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.
		Move(d3dxvShift, bUpdateVelocity);
	}
}

// 플레이어를 로컬 x축, y축, z축을 중심으로 회전
void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	DWORD nCurrentCameraMode = m_pCamera->GetMode();

	// 1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 필요
	if ((nCurrentCameraMode == FIRST_PERSON_CAMERA) || (nCurrentCameraMode == THIRD_PERSON_CAMERA))
	{
		// 로컬 x축을 중심으로 회전하는 것은 고개를 앞뒤로 숙이는 동작에 해당
		// 그러므로 x축을 중심으로 회전하는 각도는 -89.0~+89.0도 사이로 제한한다.
		// x는 현재의 m_fPitch에서 실제 회전하는 각도이므로 x만큼 회전한 다음 
		// Pitch가 +89도 보다 크거나 -89도 보다 작으면 m_fPitch가 +89도 또는 -89도가 되도록 회전각도(x)를 수정한다.
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }
			if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		// 로컬 y축은 회전 각도에 제한이 걸리지 않음 (몸을 돌리는 것)
		if (y != 0.0f)
		{
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		// 로컬 z축으로 회전하는 것은 고개를 기웃거리는 동작
		// -20 ~ 20도 사이로 결정해주는 게 일반적이다.
		if (z != 0.0f)
		{
			m_fRoll += z;
			if (m_fRoll > +20.0f) { z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		// 카메라를 x, y, z 만큼 회전한다. 플레이어를 회전하면 카메라가 회전하게 된다.
		m_pCamera->Rotate(x, y, z);

		// 플레이어를 회전한다. 1인칭 카메라 또는 3인칭 카메라에서 플레이어의 회전은 로컬 y-축에서만 일어난다.
		// 플레이어의 로컬 y-축(Up 벡터)을 기준으로 로컬 z-축(Look 벡터)와 로컬 x-축(Right 벡터)을 회전시킨다. 
		// 기본적으로 Up 벡터를 기준으로 회전하는 것은 플레이어가 똑바로 서있는 것을 가정한다는 의미이다.
		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}
	else if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		// 스페이스-쉽 카메라에서 플레이어의 회전은 회전 각도의 제한이 없다. 그리고 모든 축을 중심으로 회전을 할 수 있다.
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(x));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		}
		if (y != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
		if (z != 0.0f)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvLook, (float)D3DXToRadian(z));
			D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
			D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
		}
	}

	// 회전으로 인해 플레이어의 로컬 x축, y축, z축이 서로 직교하지 않을 수 있으므로 
	// z축(LookAt 벡터)을 기준으로 하여 서로 직교하고 단위벡터가 되도록 한다.
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
}

void CPlayer::Move(const D3DXVECTOR3& d3dxvShift, bool bUpdateVelocity)
{
	// bUpdateVelocity가 참이면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bUpdateVelocity)
	{
		m_d3dxvVelocity += d3dxvShift;
	}
	else
	{
		// 플레이어를 현재 위치 벡터에서 d3dxvShift 벡터 만큼 이동한다.
		D3DXVECTOR3 d3dxvPosition = m_d3dxvPosition + d3dxvShift;
		m_d3dxvPosition = d3dxvPosition;
		// 플레이어의 위치가 변경되었으므로 카메라의 위치도 d3dxvShift 벡터 만큼 이동한다.
		m_pCamera->Move(d3dxvShift);
	}
}

void CPlayer::Update(float fTimeElapsed)
{
	// 플레이어의 속도 벡터의 XZ-성분의 크기를 구한다. 
	// 이것이 XZ-평면의 최대 속력보다 크면 속도 벡터의 x와 z축 방향 성분을 조정한다.
	float fLength = sqrtf(m_d3dxvVelocity.x * m_d3dxvVelocity.x + m_d3dxvVelocity.z * m_d3dxvVelocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > fMaxVelocityXZ)
	{
		m_d3dxvVelocity.x *= (fMaxVelocityXZ / fLength);
		m_d3dxvVelocity.z *= (fMaxVelocityXZ / fLength);
	}
	
	// 플레이어의 속도 벡터의 Y-성분의 크기를 구한다. 
	// 이것이 Y 축 방향의 최대 속력보다 크면 속도 벡터의 y-방향 성분을 조정한다.
	fLength = sqrtf(m_d3dxvVelocity.y * m_d3dxvVelocity.y);
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	if (fLength > fMaxVelocityY) m_d3dxvVelocity.y *= (fMaxVelocityY / fLength);

	// 플레이어를 속도 벡터 만큼 실제로 이동한다(카메라도 이동될 것이다). 
	Move(m_d3dxvVelocity, false);

	// 플레이어의 위치가 변경될 때 추가로 수행할 작업을 수행한다. 예를 들어, 플레이어의 위치가 변경되었지만 플레이어 객체에는 지형(Terrain)의 정보가 없다. 플레이어의 새로운 위치가 유효한 위치가 아닐 수도 있고 또는 플레이어의 충돌 검사 등을 수행할 필요가 있다. 이러한 상황에서 플레이어의 위치를 유효한 위치로 다시 변경할 수 있다.*/
	if (m_pPlayerUpdatedContext) OnPlayerUpdated(fTimeElapsed);

	DWORD nCurrentCameraMode = m_pCamera->GetMode();
	// 플레이어의 위치가 변경되었으므로 카메라의 상태를 갱신한다.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(fTimeElapsed);
	// 카메라의 위치가 변경될 때 추가로 수행할 작업을 수행한다. 
	if (m_pCameraUpdatedContext) OnCameraUpdated(fTimeElapsed);
	// 카메라가 3인칭 카메라이면 카메라가 변경된 플레이어 위치를 바라보도록 한다.
	if (nCurrentCameraMode == THIRD_PERSON_CAMERA) m_pCamera->SetLookAt(m_d3dxvPosition);
	// 카메라의 카메라 변환 행렬을 다시 생성한다.
	m_pCamera->RegenerateViewMatrix();

	// 플레이어의 속도 벡터가 마찰력 때문에 감속이 되어야 한다면 감속 벡터를 생성한다. 속도 벡터의 반대 방향 벡터를 구하고 단위 벡터로 만든다. 마찰 계수를 시간에 비례하도록 하여 마찰력을 구한다. 단위 벡터에 마찰력을 곱하여 감속 벡터를 구한다. 속도 벡터에 감속 벡터를 더하여 속도 벡터를 줄인다. 마찰력이 속력보다 크면 속력은 0이 될 것이다.*/
	D3DXVECTOR3 d3dxvDeceleration = -m_d3dxvVelocity;
	D3DXVec3Normalize(&d3dxvDeceleration, &d3dxvDeceleration);
	fLength = D3DXVec3Length(&m_d3dxvVelocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);
	if (fDeceleration > fLength) fDeceleration = fLength;
	m_d3dxvVelocity += d3dxvDeceleration * fDeceleration;
}

CCamera *CPlayer::OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	CCamera *pNewCamera = NULL;
	// 새로운 카메라의 모드에 따라 카메라를 새로 생성한다.
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}
	// 현재 카메라의 모드가 스페이스-쉽 모드의 카메라이고 새로운 카메라가 1인칭 또는 3인칭 카메라이면
	// 플레이어의 Up 벡터를 월드좌표계의 y-축 방향 벡터(0, 1, 0)이 되도록 한다. 
	// 즉, 똑바로 서도록 한다. 그리고 스페이스-쉽 카메라의 경우 플레이어의 이동에는 제약이 없다. 특히, y-축 방향의 움직임이 자유롭다.
	// 그러므로 플레이어의 위치는 공중(위치 벡터의 y-좌표가 0보다 크다)이 될 수 있다. 이때 새로운 카메라가 1인칭 또는 3인칭 카메라이면
	// 플레이어의 위치는 지면이 되어야 한다. 그러므로 플레이어의 Right 벡터와 Look 벡터의 y 값을 0으로 만든다.
	// 이제 플레이어의 Right 벡터와 Look 벡터는 단위벡터가 아니므로 정규화한다.
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_d3dxvRight.y = 0.0f;
		m_d3dxvLook.y = 0.0f;
		D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
		D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		// Look 벡터와 월드좌표계의 z-축(0, 0, 1)이 이루는 각도(내적=cos)를 계산하여 플레이어의 y-축의 회전 각도 m_fYaw로 설정한다.
		m_fYaw = (float)D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_d3dxvLook)));
		if (m_d3dxvLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		// 새로운 카메라의 모드가 스페이스-쉽 모드의 카메라이고 현재 카메라 모드가 1인칭 또는 3인칭 카메라이면 플레이어의 로컬 축을 현재 카메라의 로컬 축과 같게 만든다.
		m_d3dxvRight = m_pCamera->GetRightVector();
		m_d3dxvUp = m_pCamera->GetUpVector();
		m_d3dxvLook = m_pCamera->GetLookVector();
	}

	if (pNewCamera)
	{
		// 기존 카메라가 없으면 새로운 카메라를 위한 쉐이더 변수를 생성한다.
		if (!m_pCamera) pNewCamera->CreateShaderVariables(pd3dDevice);
		pNewCamera->SetMode(nNewCameraMode);
		// 현재 카메라를 사용하는 플레이어 객체를 설정한다.
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera) delete m_pCamera;

	return pNewCamera;
}

void CPlayer::OnPlayerUpdated(float fTimeElapsed)
{
}

void CPlayer::OnCameraUpdated(float fTimeElapsed)
{
}

void CPlayer::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
}

// 플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다. 
// 플레이어의 Right 벡터가 월드 변환 행렬의 첫 번째 행 벡터, Up 벡터가 두 번째 행 벡터, Look 벡터가 세 번째 행 벡터, 플레이어의 위치 벡터가 네 번째 행 벡터가 된다.
void CPlayer::OnPrepareRender()
{
	m_d3dxmtxWorld._11 = m_d3dxvRight.x;
	m_d3dxmtxWorld._12 = m_d3dxvRight.y;
	m_d3dxmtxWorld._13 = m_d3dxvRight.z;
	m_d3dxmtxWorld._21 = m_d3dxvUp.x;
	m_d3dxmtxWorld._22 = m_d3dxvUp.y;
	m_d3dxmtxWorld._23 = m_d3dxvUp.z;
	m_d3dxmtxWorld._31 = m_d3dxvLook.x;
	m_d3dxmtxWorld._32 = m_d3dxvLook.y;
	m_d3dxmtxWorld._33 = m_d3dxvLook.z;
	m_d3dxmtxWorld._41 = m_d3dxvPosition.x;
	m_d3dxmtxWorld._42 = m_d3dxvPosition.y;
	m_d3dxmtxWorld._43 = m_d3dxvPosition.z;
}

CCameraPlayer::CCameraPlayer(ID3D11Device *pd3dDevice)
{
}

CCameraPlayer::~CCameraPlayer()
{
}

// 카메라를 변경할 때 호출되는 함수이다. nNewCameraMode는 새로 설정할 카메라 모드이다. 
void CCameraPlayer::ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode) return;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		SetFriction(200.0f);
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case SPACESHIP_CAMERA:	
		SetFriction(125.0f);
		SetMaxVelocityXZ(400.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(D3DXVECTOR3(0.0f, 40.0f, -60.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break;
	}
	// 카메라 정보를 시간에 따라 갱신한다.
	Update(fTimeElapsed);
}