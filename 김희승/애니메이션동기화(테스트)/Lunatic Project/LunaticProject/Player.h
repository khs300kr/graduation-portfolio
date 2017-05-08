#pragma once
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
protected:
	CCamera *m_pCamera;

	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvRight;
	D3DXVECTOR3 m_d3dxvUp;
	D3DXVECTOR3 m_d3dxvLook;

	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	D3DXVECTOR3 m_d3dxvVelocity;
	float m_fMaxVelocityXZ;
	float m_fMaxVelocityY;
	//플레이어에 작용하는 마찰력을 나타낸다.
	float m_fFriction;

	//플레이어의 위치가 바뀔 때마다 호출되는 OnPlayerUpdated() 함수에서 사용하는 데이터이다.
	LPVOID m_pPlayerUpdatedContext;
	//카메라의 위치가 바뀔 때마다 호출되는 OnCameraUpdated() 함수에서 사용하는 데이터이다.
	LPVOID m_pCameraUpdatedContext;


public:
	CPlayer(int nMeshes = 1);
	virtual ~CPlayer();

	// 플레이어의 현재 카메라를 설정하고 반환하는 멤버 함수를 선언한다.
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }
	CCamera *GetCamera() { return(m_pCamera); }

	// 플레이어의 상수 버퍼를 생성하고 갱신하는 멤버 함수를 선언한다.
	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	D3DXVECTOR3 GetPosition() { return(m_d3dxvPosition); }
	D3DXVECTOR3 GetLookVector() { return(m_d3dxvLook); }
	D3DXVECTOR3 GetUpVector() { return(m_d3dxvUp); }
	D3DXVECTOR3 GetRightVector() { return(m_d3dxvRight); }

	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(const D3DXVECTOR3& d3dxvVelocity) { m_d3dxvVelocity = d3dxvVelocity; }

	void SetPosition(const D3DXVECTOR3& d3dxvPosition) { Move((d3dxvPosition - m_d3dxvPosition), false); }

	const D3DXVECTOR3& GetVelocity() const { return(m_d3dxvVelocity); }
	float GetYaw() const { return(m_fYaw); }
	float GetPitch() const { return(m_fPitch); }
	float GetRoll() const { return(m_fRoll); }

	// 플레이어를 이동하는 함수이다.
	void Move(ULONG nDirection, float fDistance, bool bVelocity = false);
	void Move(const D3DXVECTOR3& d3dxvShift, bool bVelocity = false);
	void Move(float fxOffset = 0.0f, float fyOffset = 0.0f, float fzOffset = 0.0f);
	// 플레이어를 회전하는 함수이다.
	void Rotate(float x, float y, float z);

	// 플레이어의 위치와 회전 정보를 경과 시간에 따라 갱신하는 함수이다.
	void Update(float fTimeElapsed);

	//플레이어의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정하는 함수이다.
	virtual void OnPlayerUpdated(float fTimeElapsed);
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	//카메라의 위치가 바뀔 때마다 호출되는 함수와 그 함수에서 사용하는 정보를 설정하는 함수이다.
	virtual void OnCameraUpdated(float fTimeElapsed);
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }

	// 카메라를 변경할 때 호출되는 함수이다.
	CCamera *OnChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	// 플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다.
	virtual void OnPrepareRender();
};

class CCameraPlayer : public CPlayer
{
public:
	CCameraPlayer(ID3D11Device *pd3dDevice);
	virtual ~CCameraPlayer();

	virtual void ChangeCamera(ID3D11Device *pd3dDevice, DWORD nNewCameraMode, float fTimeElapsed);
	virtual void OnPrepareRender(){}
};
