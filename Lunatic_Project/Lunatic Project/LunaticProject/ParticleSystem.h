#pragma once

struct CB_PARTICLEINFO
{
	D3DXVECTOR3 m_d3dxvEmitPosition;
	float m_fGameTime;
	D3DXVECTOR3 m_d3dxvEmitDirection;
	float m_fTimeStep;
	D3DXVECTOR3 m_d3dxvAcceleration;
};

struct CParticleVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvVelocity;
	D3DXVECTOR2 m_d3dxvSize;
	float m_fAge;
	UINT m_nType;
};

class CParticleSystem
{
private:
	UINT m_nMaxParticles;		// 최대 파티클
	bool m_bInitializeParticle;	// Emitter 파티클 생성 신호
	float m_fGameTime;			// ElapsedTime 저장
	float m_fTimeStep;			// 흘러간 시간 기록
	float m_fAge;				// 파티클의 소멸 Age
	D3DXVECTOR3 m_d3dxvAcceleration;	// 파티클 속도
	D3DXVECTOR3	m_d3dxvEmitPosition;	// 파티클 포지션
	D3DXVECTOR3 m_d3dxvEmitDirection;	// 파티클 방향
	
	ID3D11Buffer *m_pd3dcbParticleInfo;

	UINT m_nOffset;
	UINT m_nStride;

	// 이하는 쉐이더 설정과 관련된 객체들이다.
	ID3D11Buffer *m_pd3dInitialVertexBuffer;	// 시작점 버텍스버퍼
	ID3D11Buffer *m_pd3dStreamOutVertexBuffer;	// 스트림아웃 버텍스버퍼
	ID3D11Buffer *m_pd3dDrawVertexBuffer;		//

	ID3D11InputLayout		*m_pd3dVertexLayout;
	ID3D11VertexShader		*m_pd3dVertexShader;
	ID3D11GeometryShader	*m_pd3dGeometryShader;
	ID3D11PixelShader		*m_pd3dPixelShader;
	ID3D11VertexShader		*m_pd3dSOVertexShader;	// StreamOut용도의 VS
	ID3D11GeometryShader	*m_pd3dSOGeometryShader;
	ID3D11RasterizerState   *m_pd3dRasterizerState;

	ID3D11DepthStencilState *m_pd3dSODepthStencilState;
	ID3D11DepthStencilState *m_pd3dDepthStencilState;
	ID3D11BlendState		*m_pd3dBlendState;

	ID3D11ShaderResourceView *m_pd3dsrvRandomTexture;

	ID3D11SamplerState *m_pd3dSamplerState;
	ID3D11ShaderResourceView *m_pd3dsrvTextureArray;

public:
	CParticleSystem();
	~CParticleSystem();
	void Initialize(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView *pd3dsrvTexArray, ID3D11ShaderResourceView *pd3dsrvRandomTexture, UINT nMaxParticles);
	void CreateParticle(ID3D11Device *pd3dDevice, D3DXVECTOR3 &pd3dxvPosition, D3DXVECTOR3 &pd3dxvDirection, D3DXVECTOR3 &pd3dxvAccelerator);
	ID3D11ShaderResourceView* CreateRandomTexture1DSRV(ID3D11Device *pd3dDevice);
	void Update(float fTimeStep, float fElapsedTime);
	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void ParticleRestart() { m_bInitializeParticle = true; }

	// 이하는 쉐이더 설정과 관련된 함수들이다.
	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);
	void CreateShader(ID3D11Device *pd3dDevice);
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);
	void CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dSOGeometryShader);

	ID3D11ShaderResourceView* CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures);

};

