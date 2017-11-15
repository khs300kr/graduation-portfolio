#include "stdafx.h"
#include "UIShader.h"


CUIShader::CUIShader()
{
}

CUIShader::~CUIShader()
{
	delete(m_pUIMesh);
	delete(m_pTexture);
}

void CUIShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputElements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputElements);

	CreateVertexShaderFromFile(pd3dDevice, L"Client/UI.fx", "VSUI", "vs_5_0", &m_pd3dVertexShader, d3dInputElements, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Client/UI.fx", "PSUI", "ps_5_0", &m_pd3dPixelShader);
}

void CUIShader::CreateMesh(ID3D11Device *pd3dDevice)
{
	m_pUIMesh = new CTextureToScreenRectMesh(pd3dDevice);

	m_pTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER_STATE);
	//m_pTexture->SetSampler(0, );
}

void CUIShader::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
	m_pTexture->SetTexture(0, pShaderResourceView);
}

void CUIShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, nullptr, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, nullptr, 0);
}

void CUIShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);

	m_pTexture->UpdateShaderVariable(pd3dDeviceContext);

	m_pUIMesh->Render(pd3dDeviceContext);
}