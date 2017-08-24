#include "stdafx.h"
#include "UIObject.h"
#include "Scene.h"
#include "UIShader.h"

CUIObject::CUIObject(ID3D11Device *pd3dDevice) 
{
	//m_pMaterial = new CMaterial();
		
	/*ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);*/


	//CTexture *pTexture = new CTexture(1, 1, PS_SLOT_TEXTURE, PS_SLOT_SAMPLER_STATE);
	//pTexture->SetTexture(0, tag);
	//pTexture->SetSampler(0, pd3dSamplerState);

	//m_pMaterial->SetTexture(pTexture);
}

CUIObject::~CUIObject()
{
	delete(m_pMesh);
	delete(m_pTexture);
}

/*
bool CUIObject::IsCollision(POINT mousePos)
{
	if (m_ptStartPos.x * TEXT_MGR->GetResizeRatio().x <= mousePos.x && mousePos.x <= m_ptEndPos.x * TEXT_MGR->GetResizeRatio().x) {
		if (m_ptStartPos.y * TEXT_MGR->GetResizeRatio().y <= mousePos.y && mousePos.y <= m_ptEndPos.y * TEXT_MGR->GetResizeRatio().y)
			return true;
	}
	return false;
*/

void CUIObject::Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos)
{
	m_ptStartPos = startPos;
	m_ptEndPos = endPos;

	m_pMesh = new CUIMesh(pDevice, m_ptStartPos, m_ptEndPos, zPos);
}

void CUIObject::Update()
{
	Initialize(pUIDevice, m_ptStartPos, m_ptEndPos, 0.3f);
}	

void CUIObject::SetScore(POINT _sPoint, POINT _ePoint)
{
	Initialize(pUIDevice, _sPoint, _ePoint, 0.4f);
}

void CUIObject::SetGamer(POINT _sPoint, POINT _ePoint)
{
	Initialize(pUIDevice, _sPoint, _ePoint, 0.5f);
}

void CUIObject::SetHp(POINT _sPoint, POINT _ePoint)
{
	Initialize(pUIDevice, _sPoint, _ePoint, 0.3f);
}

void CUIObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	if (m_pTexture) m_pTexture->UpdateShaderVariable(pDeviceContext);

	m_pMesh->Render(pDeviceContext);
}

// ============================================================================================================================  //
// ======================================================== UI Manager ========================================================  //

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
	delete(m_pUIShader);
	delete(m_pBackGroundUI);

	for (auto& obj : m_vecUIObject)
		delete(obj);
}

CUIObject* CUIManager::GetUIObject(TextureTag tag)
{
	for (auto& obj : m_vecUIObject)
		if (obj->GetTag() == tag)
			return obj;
}

void CUIManager::Initialize(ID3D11Device* pDevice)
{
	m_pUIShader = new CUIShader();
	m_pUIShader->CreateShader(pDevice);

	// TransparentBS
	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pDevice->CreateBlendState(&transparentDesc, &g_pTransparentBS);
}

TextureTag CUIManager::FindCollisionUIObject(POINT mousePos)
{
	for (auto& uiObj : m_vecUIObject) {
		if (uiObj->GetActive()) {
//			if (uiObj->IsCollision(mousePos)) 
				return uiObj->GetTag();
		}
	}

//	return TextureTag::eNone;
}

void CUIManager::RenderAll(ID3D11DeviceContext* pDeviceContext)
{


	m_pUIShader->OnPrepareRender(pDeviceContext);

	pDeviceContext->OMSetBlendState(g_pTransparentBS, NULL, 0xffffffff);
	
	if(m_pBackGroundUI) 
		m_pBackGroundUI->Render(pDeviceContext);

	for (auto& uiObj : m_vecUIObject)
		uiObj->Render(pDeviceContext);
	
	/*pDeviceContext->RSSetState(STATEOBJ_MGR->g_pDefaultRS);*/
	pDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}