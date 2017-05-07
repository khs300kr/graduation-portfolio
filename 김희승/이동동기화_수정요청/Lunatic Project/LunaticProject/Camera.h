#pragma once
#include "Mesh.h"

#define FIRST_PERSON_CAMERA	0x01
#define SPACESHIP_CAMERA	0x02
#define THIRD_PERSON_CAMERA	0x03

// ��� ���۸� ���� ����ü
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
	D3DXVECTOR3 m_d3dxvLookAtWorld; // ī�޶� �ٶ󺸴� ��(���� ��ǥ)
	D3DXVECTOR3 m_d3dxvOffset;		// 3��Ī ī�޶��� ������ ����
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;
	float m_fTimeLag;
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;

	D3DXPLANE m_d3dxFrustumPlanes[6];	// ����ü �ø��� ���

	D3D11_VIEWPORT m_d3dViewport;

	// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ��� ���� �������̽� ������
	ID3D11Buffer *m_pd3dcbCamera;
	// ī�޶� ����� �÷��̾� ��ü�� ���� ������ - ���Ŀ��� ���� �ʿ� ������.
	CPlayer *m_pPlayer;
	// ī�޶��� ����(1��Ī ī�޶�, �����̽�-�� ī�޶�, 3��Ī ī�޶�) - ���� ����
	DWORD m_nMode;

public:
	CCamera(CCamera *pCamera);
	virtual ~CCamera();

	// ��� ���۸� �����ϰ� ������ �����ϴ� ��� �Լ��� ����
	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	// ī�޶� ��ȯ����� ����
	void GenerateViewMatrix();
	// ���� ��� �� �Ǽ������� ����Ȯ���� �����ϱ� ����, ���� ������ �ٽ� �����ִ� �Լ�
	void RegenerateViewMatrix();

	// ī�޶� ��ȯ ��� & ���� ��ȯ ����� ����
	void GenerateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp);
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);

	// Set & Get �Լ���...
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

	// ī�޶� �̵�, ȸ��, �׸��� �׿� ���� ���� �Լ�.
	virtual void Move(const D3DXVECTOR3& d3dxvShift) { m_d3dxvPosition += d3dxvShift; }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(float fTimeElapsed) { }
	virtual void SetLookAt(D3DXVECTOR3& vLookAt) { }

	// ����ü �ø��� ���� �Լ�.
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