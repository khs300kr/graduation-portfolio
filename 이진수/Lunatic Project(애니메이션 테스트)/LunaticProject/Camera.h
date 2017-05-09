#pragma once
#include "Mesh.h"

#define FIRST_PERSON_CAMERA	0x01
#define SPACESHIP_CAMERA	0x02
#define THIRD_PERSON_CAMERA	0x03

// 상수 버퍼를 위한 구조체
struct VS_CB_CAMERA
{
	D3DXMATRIX	m_d3dxmtxView;
	D3DXMATRIX	m_d3dxmtxProjection;
};

class CPlayer;

class CCamera
{
protected:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;
	D3DXVECTOR3 m_d3dxvLookAtWorld; // 카메라가 바라보는 점(월드 좌표)
	D3DXVECTOR3 m_d3dxvOffset;		// 3인칭 카메라의 오프셋 벡터
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;
	float m_fTimeLag;
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	D3DXPLANE m_d3dxFrustumPlanes[6];	// 절두체 컬링용 평면

	D3D11_VIEWPORT m_d3dViewport;

	// 카메라 변환 행렬과 투영 변환 행렬을 위한 상수 버퍼 인터페이스 포인터
	ID3D11Buffer *m_pd3dcbCamera;
	// 카메라에 연결된 플레이어 객체에 대한 포인터 - 추후에는 굳이 필요 없을듯.
	CPlayer *m_pPlayer;
	// 카메라의 종류(1인칭 카메라, 스페이스-쉽 카메라, 3인칭 카메라) - 추후 삭제
	DWORD m_nMode;

public:
	CCamera(CCamera *pCamera);
	virtual ~CCamera();

	// 상수 버퍼를 생성하고 내용을 갱신하는 멤버 함수를 선언
	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	// 카메라 변환행렬을 생성
	void GenerateViewMatrix();
	// 잦은 행렬 시 실수연산의 부정확성을 방지하기 위해, 축의 직교를 다시 맞춰주는 함수
	void RegenerateViewMatrix();

	// 카메라 변환 행렬 & 투영 변환 행렬을 생성
	void GenerateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// Set & Get 함수들...
	void SetLookAtPosition(D3DXVECTOR3 d3dxvLookAtWorld) { m_d3dxvLookAtWorld = d3dxvLookAtWorld; }
	void SetMode(DWORD nMode) { m_nMode = nMode; }
	void SetOffset(D3DXVECTOR3 d3dxvOffset) { m_d3dxvOffset = d3dxvOffset; m_d3dxvPosition += d3dxvOffset; }
	void SetPlayer(CPlayer *pPlayer) { m_pPlayer = pPlayer; }
	void SetPosition(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);

	ID3D11Buffer	*GetCameraConstantBuffer() { return m_pd3dcbCamera; }
	D3DXVECTOR3&	GetLookAtPosition() { return m_d3dxvLookAtWorld; }
	DWORD			GetMode() { return m_nMode; }
	D3DXVECTOR3&	GetOffset() { return(m_d3dxvOffset); }
	CPlayer			*GetPlayer() { return m_pPlayer; }
	D3DXVECTOR3&	GetPosition() { return m_d3dxvPosition; }
	D3DXMATRIX		GetProjectionMatrix() { return m_d3dxmtxProjection; }
	float			GetTimeLag() { return m_fTimeLag; }
	D3D11_VIEWPORT	GetViewport() { return m_d3dViewport; }
	D3DXMATRIX		GetViewMatrix() { return m_d3dxmtxView; }

	float& GetPitch() { return m_fPitch; }
	float& GetRoll() { return m_fRoll; }
	float& GetYaw() { return m_fYaw; }

	D3DXVECTOR3& GetRightVector() { return m_d3dxvRight; }
	D3DXVECTOR3& GetUpVector() { return m_d3dxvUp; }
	D3DXVECTOR3& GetLookVector() { return m_d3dxvLook; }

	// 카메라 이동, 회전, 그리고 그에 따른 갱신 함수.
	virtual void Move(const D3DXVECTOR3& d3dxvShift) { m_d3dxvPosition += d3dxvShift; }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(float fTimeElapsed) { }
	virtual void SetLookAt(D3DXVECTOR3& vLookAt) { }

	// 절두체 컬링을 위한 함수.
	void CalculateFrustumPlanes();
	bool IsInFrustum(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	bool IsInFrustum(AABB *pAABB);
};

class CSpaceShipCamera : public CCamera
{
public:
	CSpaceShipCamera(CCamera *pCamera);
	virtual ~CSpaceShipCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CFirstPersonCamera : public CCamera
{
public:
	CFirstPersonCamera(CCamera *pCamera);
	virtual ~CFirstPersonCamera() { }

	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CCamera *pCamera);
	virtual ~CThirdPersonCamera() { }

	virtual void Update(float fTimeScale);
	virtual void SetLookAt(D3DXVECTOR3& vLookAt);
};