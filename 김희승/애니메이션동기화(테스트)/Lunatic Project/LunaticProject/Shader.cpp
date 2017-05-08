#include "stdafx.h"
#include "Shader.h"

// ���� ��ȯ ����� ���� ��� ���۴� ���̴� ��ü�� ����(static) ������ ����̴�.
ID3D11Buffer *CShader::m_pd3dcbWorldMatrix = NULL;
ID3D11Buffer *CIlluminatedShader::m_pd3dcbMaterial = NULL;

CShader::CShader(int nObjects)
{
	m_ppObjects = NULL;
	m_nObjects = nObjects;
	if (m_nObjects > 0)
	{
		m_ppObjects = new CGameObject*[m_nObjects];
		for (int i = 0; i < m_nObjects; i++) m_ppObjects[i] = NULL;
	}


	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
	m_nIndexToAdd = 0;
}


CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
}


void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
	ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;

	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"CreateVertexShaderFromFile Error", L"ERROR_VS", MB_OK);
		exit(1);
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	// ����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		// �����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
	else
	{
		MessageBox(NULL, L"CreatePixelShaderFromFile Error", L"ERROR_PS", MB_OK);
		exit(1);
	}
}


void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
}


void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
	// ���̴� ��ü�� ��� ���� ��ü���� �������Ѵ�.
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			if (m_ppObjects[j]->IsVisible(pCamera))
			{
				m_ppObjects[j]->Render(pd3dDeviceContext, pCamera);
			}
		}
	}
}


void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	// ���� ��ȯ ����� ���� ��� ���۸� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbWorldMatrix);
}


void CShader::ReleaseShaderVariables()
{
	// ���� ��ȯ ����� ���� ��� ���۸� ��ȯ
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
}


void CShader::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	// ���� ��ȯ ����� ��� ���ۿ� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);

	// ��� ���۸� ����̽��� ����(VS_SLOT_WORLD_MATRIX)�� �����Ѵ�.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}


void CShader::BuildObjects(ID3D11Device *pd3dDevice)
{
}


void CShader::ReleaseObjects()
{
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture) m_pTexture->Release();

	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}


void CShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}


void CShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
}

void CShader::AddObject(CGameObject *pGameObject)
{
	m_ppObjects[m_nIndexToAdd++] = pGameObject;
	if (pGameObject) pGameObject->AddRef();
}

//
// CPlayerShader
//////////////////////////////////////////////////////////////////////////////////////////
CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

void CPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
}

void CPlayerShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];
	
	CCameraPlayer *pCameraPlayer = new CCameraPlayer(pd3dDevice);
	pCameraPlayer->SetMesh(NULL);
	pCameraPlayer->CreateShaderVariables(pd3dDevice);
	pCameraPlayer->ChangeCamera(pd3dDevice, THIRD_PERSON_CAMERA, 0.0f);

	m_ppObjects[0] = pCameraPlayer;
}



//
// CInstancingShader
//////////////////////////////////////////////////////////////////////////////////////////
CInstancingShader::CInstancingShader(int nObjects)
{
	m_pd3dInstanceBuffer = NULL;
	m_pMaterial = NULL;
	m_pTexture = NULL;

	m_ppObjects = NULL;
	m_nObjects = nObjects;
	if (m_nObjects > 0)
	{
		m_ppObjects = new CGameObject*[m_nObjects];
		for (int i = 0; i < m_nObjects; i++) m_ppObjects[i] = NULL;
	}

	m_nIndexToAdd = 0;
}

CInstancingShader::~CInstancingShader()
{
	if (m_pd3dInstanceBuffer) m_pd3dInstanceBuffer->Release();
}

void CInstancingShader::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CInstancingShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCEPOS", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 3, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"BaseEffect.fx", "VSInstancedTexturedLightingColor", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"BaseEffect.fx", "PSInstancedTexturedLightingColor", "ps_5_0", &m_pd3dPixelShader);
}

ID3D11Buffer *CInstancingShader::CreateInstanceBuffer(ID3D11Device *pd3dDevice, int nObjects, UINT nBufferStride, void *pBufferData)
{
	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	// ������ �ʱ�ȭ �����Ͱ� ������ ���� ���۷� �����Ѵ�.
	// (���߿� ������ �Ͽ� ������ ä��ų� ����)
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;// (pBufferData) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nBufferStride * nObjects;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;// (pBufferData) ? 0 : D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, (pBufferData) ? &d3dBufferData : NULL, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);
}

void CInstancingShader::BuildObjects(ID3D11Device *pd3dDevice, CMaterial *pMaterial, CTexture *pTexture)
{
	m_pMaterial = pMaterial;
	if (pMaterial) pMaterial->AddRef();
	m_pTexture = pTexture;
	if (pTexture) pTexture->AddRef();

	m_nInstanceBufferStride = sizeof(D3DXMATRIX);
	m_nInstanceBufferOffset = 0;

	m_pd3dInstanceBuffer = CreateInstanceBuffer(pd3dDevice, m_nObjects, m_nInstanceBufferStride, NULL);
	m_pMesh->AssembleToVertexBuffer(1, &m_pd3dInstanceBuffer, &m_nInstanceBufferStride, &m_nInstanceBufferOffset);
}


void CInstancingShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	if (m_pMaterial) CIlluminatedShader::UpdateShaderVariable(pd3dDeviceContext, &m_pMaterial->m_Material);
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pd3dDeviceContext);

	int test = 0;

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	int nInstance = 0;
	pd3dDeviceContext->Map(m_pd3dInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	D3DXMATRIX *pd3dxmtxInstances = (D3DXMATRIX *)d3dMappedResource.pData;
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			// ������ü ��ü�� ī�޶� ���� ��(������ü ��ü�� �ٿ�� �ڽ��� ����ü�� ������ ��) 
			// �ν��Ͻ� �����͸� �ν��Ͻ� ���۷� ����
			if (m_ppObjects[j]->IsVisible(pCamera))
			{
				D3DXMatrixTranspose(&pd3dxmtxInstances[nInstance++], &m_ppObjects[j]->m_d3dxmtxWorld);
				test++;
			}
		}
	}
	printf("��������������������������������\r[DEBUG] �������� ��ü ����: %d��", test);
	pd3dDeviceContext->Unmap(m_pd3dInstanceBuffer, 0);

	// ī�޶� ���̴� ������ü�鸸 �ν��Ͻ��� �Ѵ�.
	m_pMesh->RenderInstanced(pd3dDeviceContext, nInstance, 0);
}


CDiffusedShader::CDiffusedShader()
{
}

CDiffusedShader::~CDiffusedShader()
{
}

void CDiffusedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSDiffusedColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSDiffusedColor", "ps_5_0", &m_pd3dPixelShader);
}



CIlluminatedShader::CIlluminatedShader()
{
}

CIlluminatedShader::~CIlluminatedShader()
{
}

void CIlluminatedShader::CreateShader(ID3D11Device *pd3dDevice)
{
}

void CIlluminatedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}

void CIlluminatedShader::ReleaseShaderVariables()
{
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();
}

void CIlluminatedShader::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	//D3DXCOLOR *pcbMaterial = (D3DXCOLOR *)d3dMappedResource.pData;
	//memcpy(pcbMaterial, &pMaterial->m_d3dxcAmbient, sizeof(D3DXCOLOR));
	//memcpy(pcbMaterial + 1, &pMaterial->m_d3dxcDiffuse, sizeof(D3DXCOLOR));
	//memcpy(pcbMaterial + 2, &pMaterial->m_d3dxcSpecular, sizeof(D3DXCOLOR));
	//memcpy(pcbMaterial + 3, &pMaterial->m_d3dxcEmissive, sizeof(D3DXCOLOR));
	MATERIAL *pcbMaterial = (MATERIAL *)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}



CTexturedShader::CTexturedShader()
{
}

CTexturedShader::~CTexturedShader()
{
}

void CTexturedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);
	CreateVertexShaderFromFile(pd3dDevice, L"BaseEffect.fx", "VSTexturedColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"BaseEffect.fx", "PSTexturedColor", "ps_5_0", &m_pd3dPixelShader);
}



//
// CTextureIlluminatedShader Class
//////////////////////////////////////////////////////////////////////////////////////////
CTexturedIlluminatedShader::CTexturedIlluminatedShader(int nObjects = 1)
{
	m_ppObjects = NULL;

	m_nObjects = nObjects;
	if (m_nObjects > 0)
	{
		m_ppObjects = new CGameObject*[m_nObjects];
		for (int i = 0; i < m_nObjects; i++) m_ppObjects[i] = NULL;
	}

	m_pMaterial = NULL;
	m_nIndexToAdd = 0;
}

CTexturedIlluminatedShader::~CTexturedIlluminatedShader()
{
}

void CTexturedIlluminatedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);
	CreateVertexShaderFromFile(pd3dDevice, L"BaseEffect.fx", "VSTexturedLightingColor", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"BaseEffect.fx", "PSTexturedLightingColor", "ps_5_0", &m_pd3dPixelShader);
}
