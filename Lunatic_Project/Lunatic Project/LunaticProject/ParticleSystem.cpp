#pragma once
#include "stdafx.h"
#include "ParticleSystem.h"

#define PARTICLE_TYPE_EMITTER 0
#define PARTICLES			  500	

float R(float a, float b) {
	return(a + (float)(rand() / (float)RAND_MAX) * (b - a));

}

CParticleSystem::CParticleSystem()
{
	m_nMaxParticles = 0;	
	m_bInitializeParticle = false;
	m_fGameTime = 0.0f;			
	m_fTimeStep = 0.0f;		
	m_fAge = 0.0f;
	m_nOffset = 0;
	m_nStride = 0;
	m_d3dxvAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvEmitDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pd3dcbParticleInfo = NULL;
	
	m_pd3dInitialVertexBuffer = NULL;
	m_pd3dStreamOutVertexBuffer = NULL;
	m_pd3dDrawVertexBuffer = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dVertexShader = NULL;
	m_pd3dGeometryShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dSOVertexShader = NULL;
	m_pd3dSOGeometryShader = NULL;
	m_pd3dRasterizerState = NULL;
	m_pd3dSODepthStencilState = NULL;
	m_pd3dDepthStencilState = NULL;
	m_pd3dBlendState = NULL;
	m_pd3dsrvRandomTexture = NULL;
	m_pd3dSamplerState = NULL;
	m_pd3dsrvTextureArray = NULL;
}

CParticleSystem::~CParticleSystem()
{
	if (m_pd3dInitialVertexBuffer) m_pd3dInitialVertexBuffer->Release();
	if (m_pd3dStreamOutVertexBuffer) m_pd3dStreamOutVertexBuffer->Release(); 
	if (m_pd3dDrawVertexBuffer) m_pd3dDrawVertexBuffer->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release(); 
	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dSOVertexShader) m_pd3dSOVertexShader->Release();
	if (m_pd3dSOGeometryShader) m_pd3dSOGeometryShader->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dSODepthStencilState) m_pd3dSODepthStencilState->Release();
	if (m_pd3dDepthStencilState) m_pd3dDepthStencilState->Release();
	if (m_pd3dBlendState) m_pd3dBlendState->Release();
	if (m_pd3dsrvRandomTexture) m_pd3dsrvRandomTexture->Release();
	if (m_pd3dSamplerState) m_pd3dSamplerState->Release();
	if (m_pd3dsrvTextureArray) m_pd3dsrvTextureArray->Release();
}

void CParticleSystem::Initialize(ID3D11Device *pd3dDevice, ID3D11ShaderResourceView* pd3dsrvTexArray,
	ID3D11ShaderResourceView* pd3dsrvRandomTexture, UINT nMaxParticles)
{
	m_pd3dsrvTextureArray = pd3dsrvTexArray;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Particle/particle2.bmp"), NULL, NULL, &m_pd3dsrvTextureArray, NULL);

	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &m_pd3dSamplerState);
	//샘플러 상태 추가

	//랜덤값을 위한 텍스쳐
	m_pd3dsrvRandomTexture = pd3dsrvRandomTexture;

	m_nMaxParticles = nMaxParticles;

	m_bInitializeParticle = true;

	m_fGameTime = 0;
	m_fTimeStep = 0;
	m_fAge = 0;
	
	m_d3dxvAcceleration = D3DXVECTOR3(0, 0, 0);
	m_d3dxvEmitPosition = D3DXVECTOR3(0, 0, 0);
	m_d3dxvEmitDirection = D3DXVECTOR3(0, 0, 0);

	CreateParticle(pd3dDevice, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
}
void CParticleSystem::CreateParticle(ID3D11Device *pd3dDevice, D3DXVECTOR3& Position, D3DXVECTOR3& Direction, D3DXVECTOR3& Accelerater)
{
	m_d3dxvEmitPosition = Position;
	m_d3dxvEmitDirection = Direction;
	m_d3dxvAcceleration = Accelerater;
	m_nOffset = 0;
	m_nStride = sizeof(CParticleVertex);

	CreateShaderVariables(pd3dDevice);	
}

void CParticleSystem::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateVertexShaderFromFile(pd3dDevice, L"particle.fx", "VSParticleStreamOut", "vs_5_0", &m_pd3dSOVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"particle.fx", "VSParticleDraw", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, 5, &m_pd3dVertexLayout);
	CreateSOGeometryShaderFromFile(pd3dDevice, L"particle.fx", "GSParticleStreamOut", "gs_5_0", &m_pd3dSOGeometryShader);
	CreateGeometryShaderFromFile(pd3dDevice, L"particle.fx", "GSParticleDraw", "gs_5_0", &m_pd3dGeometryShader);
	CreatePixelShaderFromFile(pd3dDevice, L"particle.fx", "PSParticleDraw", "ps_5_0", &m_pd3dPixelShader);

	// 깊이 스텐실 상태 설정
	//D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	//ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	//d3dDepthStencilDesc.DepthEnable = false;
	//d3dDepthStencilDesc.StencilEnable = false;
	//d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dSODepthStencilState);
	//d3dDepthStencilDesc.DepthEnable = true;
	//d3dDepthStencilDesc.StencilEnable = false;
	//d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	//pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);

	// 블렌드 상태 설정
	D3D11_BLEND_DESC d3dBlendStateDesc;
	ZeroMemory(&d3dBlendStateDesc, sizeof(D3D11_BLEND_DESC));
	d3dBlendStateDesc.IndependentBlendEnable = false;
	ZeroMemory(&d3dBlendStateDesc.RenderTarget[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	//d3dBlendStateDesc.AlphaToCoverageEnable = true;
	d3dBlendStateDesc.RenderTarget[0].BlendEnable = true;
	d3dBlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	d3dBlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&d3dBlendStateDesc, &m_pd3dBlendState);

	// 래스터라이즈 설정
	D3D11_RASTERIZER_DESC d3dRasterizerStateDesc;
	ZeroMemory(&d3dRasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerStateDesc.FillMode = D3D11_FILL_SOLID;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerStateDesc, &m_pd3dRasterizerState);
}

void CParticleSystem::Update(float fTimeStep, float fGameTime)
{
	m_fGameTime = fGameTime;
	m_fTimeStep = fTimeStep;
}

void CParticleSystem::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	UpdateShaderVariables(pd3dDeviceContext);
	

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->SOSetTargets(1, &m_pd3dStreamOutVertexBuffer, &m_nOffset);

	pd3dDeviceContext->VSSetShader(m_pd3dSOVertexShader, NULL, 0);//
	pd3dDeviceContext->GSSetShader(m_pd3dSOGeometryShader, NULL, 0);//
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dSODepthStencilState, 0);
	pd3dDeviceContext->GSSetSamplers(0, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->GSSetShaderResources(0, 1, &m_pd3dsrvRandomTexture);

	if (m_bInitializeParticle)
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dInitialVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->Draw(1, 0);
		m_bInitializeParticle = false;
	}
	else
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
		pd3dDeviceContext->DrawAuto();
	}
	ID3D11Buffer *pd3dBuffer = m_pd3dDrawVertexBuffer;
	m_pd3dDrawVertexBuffer = m_pd3dStreamOutVertexBuffer;
	m_pd3dStreamOutVertexBuffer = pd3dBuffer;

	ID3D11Buffer *pd3dBuffers[1] = { NULL };
	UINT pStreamOffSets[1] = { 0 };
	pd3dDeviceContext->SOSetTargets(1, pd3dBuffers, pStreamOffSets);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->GSSetShader(m_pd3dGeometryShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 0);
	pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, NULL, 0xffffffff);

	pd3dDeviceContext->PSSetSamplers(0, 1, &m_pd3dSamplerState);
	pd3dDeviceContext->PSSetShaderResources(5, 1, &m_pd3dsrvTextureArray);

	pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dDrawVertexBuffer, &m_nStride, &m_nOffset);
	pd3dDeviceContext->DrawAuto();

	pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
	pd3dDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

ID3D11ShaderResourceView* CParticleSystem::CreateRandomTexture1DSRV(ID3D11Device *pd3dDevice)
{
	D3DXCOLOR RV[1024];
	for (int i = 0; i < 1024; i++)
		RV[i] = D3DXCOLOR(R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f), R(-1.0f, 1.0f));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = RV;
	d3dSubResourceData.SysMemPitch = sizeof(D3DXCOLOR) * 1024;
	d3dSubResourceData.SysMemSlicePitch = 0;

	D3D11_TEXTURE1D_DESC d3dTextureDesc;
	ZeroMemory(&d3dTextureDesc, sizeof(D3D11_TEXTURE1D_DESC));
	d3dTextureDesc.Width = 1024;
	d3dTextureDesc.MipLevels = 1;
	d3dTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	d3dTextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	d3dTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTextureDesc.ArraySize = 1;
	ID3D11Texture1D *pd3dTexture;
	pd3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubResourceData, &pd3dTexture);
	ID3D11ShaderResourceView *pd3dsrvTexture;
	pd3dDevice->CreateShaderResourceView(pd3dTexture, NULL, &pd3dsrvTexture);
	pd3dTexture->Release();
	return(pd3dsrvTexture);
}

void CParticleSystem::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName,
	LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader,
	D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel,
		dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements,
			pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"VSERROR", L"VSERROR", MB_OK);
		exit(0);
	}
}

void CParticleSystem::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"PSERROR", L"PSERROR", MB_OK);
		exit(0);
	}
}

void CParticleSystem::CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		pd3dDevice->CreateGeometryShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dGeometryShader);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"GSERROR", L"GSERROR", MB_OK);
		exit(0);
	}
}


//typedef struct D3D11_SO_DECLARATION_ENTRY{
//	
//	UINT Stream;
//	LPCSTR SemanticName;
//	UINT SemanticIndex;
//	BYTE StartComponent;
//	BYTE ComponentCount;
//	BYTE OutputSlot;
//
//}D3D11_SO_DECLARATION_ENTRY;

//void CParticleSystem::CreateShaderVariables(ID3D11Device *pd3dDevice)
//{
//	ID3D11Buffer *m_pd3dSOBufferA;
//
//
//	D3D11_BUFFER_DESC d3dBufferDesc;
//	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
//	d3dBufferDesc.ByteWidth = MAX_PARTICLES * sizeof(PARTICLE_VERTEX);//크기를 어디서 지정해주나 했더니.
//	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
//	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dSOBufferA);
//
//}

void CParticleSystem::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	CParticleVertex particle;
	ZeroMemory(&particle, sizeof(CParticleVertex));
	D3D11_SUBRESOURCE_DATA d3dSubResourceData;
	d3dSubResourceData.pSysMem = &particle;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dInitialVertexBuffer);
	m_nStride = sizeof(CParticleVertex);
	d3dBufferDesc.ByteWidth = sizeof(CParticleVertex) * m_nMaxParticles;//PARTICLES = 500
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dStreamOutVertexBuffer);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dSubResourceData, &m_pd3dDrawVertexBuffer);
	//버퍼 두개를 만든다.

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(CB_PARTICLEINFO) + sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbParticleInfo);
	//상수버퍼를 만듬.
}

void CParticleSystem::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbParticleInfo, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);

	CB_PARTICLEINFO *pcbParticleInfo = (CB_PARTICLEINFO*)d3dMappedResource.pData;
	pcbParticleInfo->m_fGameTime = m_fGameTime;
	pcbParticleInfo->m_fTimeStep = m_fTimeStep;
	pcbParticleInfo->m_d3dxvAcceleration = m_d3dxvAcceleration;
	pcbParticleInfo->m_d3dxvEmitDirection = m_d3dxvEmitDirection;
	pcbParticleInfo->m_d3dxvEmitPosition = m_d3dxvEmitPosition;

	pd3dDeviceContext->Unmap(m_pd3dcbParticleInfo, 0);

	pd3dDeviceContext->GSSetConstantBuffers(GS_CB_SLOT_PARTICLE, 1, &m_pd3dcbParticleInfo);
}

void CParticleSystem::CreateSOGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader)
{
	HRESULT hResult;
	/*
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
	D3D11_SO_DECLARATION_ENTRY pSODecls[] = {
		{ 0, "POSITION", 0, 0, 3, 0 },
		{ 0, "VELOCITY", 0, 0, 3, 0 },
		{ 0, "SIZE", 0, 0, 2, 0 },
		{ 0, "TYPE", 0, 0, 1, 0 },
		{ 0, "AGE", 0, 0, 1, 0 }
	};
	UINT pBufferStrides[1] = { sizeof(CParticleVertex) };//개수 많이 지정해야되지않나

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreateGeometryShaderWithStreamOutput(pd3dShaderBlob->GetBufferPointer(),
			pd3dShaderBlob->GetBufferSize(),
			pSODecls,			// 스트림아웃 디클러레이션
			5,					// 엔트리 개수
			pBufferStrides,		// 버퍼 스트라이드
			1,					// 스트라이드 개수
			0,					// 래스터라이즈 스트림
			NULL, ppd3dGeometryShader);

		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"SOGSERROR", L"SOGSERROR", MB_OK);
		exit(0);
	}
}

ID3D11ShaderResourceView *CParticleSystem::CreateTexture2DArraySRV(ID3D11Device *pd3dDevice, _TCHAR(*ppstrFilePaths)[128], UINT nTextures)
{
	ID3D11ShaderResourceView* pd3dsrvParticleTexture;
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, *ppstrFilePaths, NULL, NULL, &pd3dsrvParticleTexture, NULL);
	return pd3dsrvParticleTexture;

	D3DX11_IMAGE_LOAD_INFO d3dxImageLoadInfo;
	d3dxImageLoadInfo.Width = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Height = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Depth = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.FirstMipLevel = 0;
	d3dxImageLoadInfo.MipLevels = D3DX11_FROM_FILE;
	d3dxImageLoadInfo.Usage = D3D11_USAGE_STAGING;
	d3dxImageLoadInfo.BindFlags = 0;
	d3dxImageLoadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	d3dxImageLoadInfo.MiscFlags = 0;
	d3dxImageLoadInfo.Format = DXGI_FORMAT_FROM_FILE; 
	d3dxImageLoadInfo.Filter = D3DX11_FILTER_NONE;
	d3dxImageLoadInfo.MipFilter = D3DX11_FILTER_LINEAR;
	d3dxImageLoadInfo.pSrcInfo = 0;

	ID3D11Texture2D **ppd3dTextures = new ID3D11Texture2D*[nTextures];
	for (UINT i = 0; i < nTextures; i++) D3DX11CreateTextureFromFile(pd3dDevice, ppstrFilePaths[i], &d3dxImageLoadInfo, 0, (ID3D11Resource **)&ppd3dTextures[i], 0);

	D3D11_TEXTURE2D_DESC d3dTexure2DDesc;

	ppd3dTextures[0]->GetDesc(&d3dTexure2DDesc);

	D3D11_TEXTURE2D_DESC d3dTexture2DArrayDesc;
	d3dTexture2DArrayDesc.Width = d3dTexure2DDesc.Width;
	d3dTexture2DArrayDesc.Height = d3dTexure2DDesc.Height;
	d3dTexture2DArrayDesc.MipLevels = d3dTexure2DDesc.MipLevels;
	d3dTexture2DArrayDesc.ArraySize = nTextures;
	d3dTexture2DArrayDesc.Format = d3dTexure2DDesc.Format;
	d3dTexture2DArrayDesc.SampleDesc.Count = 1;
	d3dTexture2DArrayDesc.SampleDesc.Quality = 0;
	d3dTexture2DArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DArrayDesc.CPUAccessFlags = 0;
	d3dTexture2DArrayDesc.MiscFlags = 0;

	ID3D11Texture2D *pd3dTexture2DArray;
	pd3dDevice->CreateTexture2D(&d3dTexture2DArrayDesc, 0, &pd3dTexture2DArray);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	D3D11_MAPPED_SUBRESOURCE d3dMappedTexture2D;
	for (UINT t = 0; t < nTextures; t++)
	{
		for (UINT m = 0; m < d3dTexure2DDesc.MipLevels; m++)
		{
			pd3dDeviceContext->Map(ppd3dTextures[t], m, D3D11_MAP_READ, 0, &d3dMappedTexture2D);
			pd3dDeviceContext->UpdateSubresource(pd3dTexture2DArray, D3D11CalcSubresource(m, t, d3dTexure2DDesc.MipLevels), 0, d3dMappedTexture2D.pData, d3dMappedTexture2D.RowPitch, d3dMappedTexture2D.DepthPitch);
			pd3dDeviceContext->Unmap(ppd3dTextures[t], m);
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d3dTextureSRVDesc;
	ZeroMemory(&d3dTextureSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dTextureSRVDesc.Format = d3dTexture2DArrayDesc.Format;
	d3dTextureSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	d3dTextureSRVDesc.Texture2DArray.MostDetailedMip = 0;
	d3dTextureSRVDesc.Texture2DArray.MipLevels = d3dTexture2DArrayDesc.MipLevels;
	d3dTextureSRVDesc.Texture2DArray.FirstArraySlice = 0;
	d3dTextureSRVDesc.Texture2DArray.ArraySize = nTextures;

	ID3D11ShaderResourceView *pd3dsrvTextureArray;
	pd3dDevice->CreateShaderResourceView(pd3dTexture2DArray, &d3dTextureSRVDesc, &pd3dsrvTextureArray);

	if (pd3dTexture2DArray) pd3dTexture2DArray->Release();

	for (UINT i = 0; i < nTextures; i++) if (ppd3dTextures[i]) ppd3dTextures[i]->Release();
	delete[] ppd3dTextures;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();

	return pd3dsrvTextureArray;
}