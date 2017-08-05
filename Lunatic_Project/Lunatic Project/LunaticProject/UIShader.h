#pragma once
#include "Shader.h"
#include "TextureToScreenRectMesh.h"

class CUIShader : public CShader
{
	CTextureToScreenRectMesh	*m_pUIMesh = nullptr;
	CTexture					*m_pTexture = nullptr;

public:
	CUIShader();
	virtual ~CUIShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice) override;
	void CreateMesh(ID3D11Device *pd3dDevice);

	void SetTexture(ID3D11ShaderResourceView* pShaderResourceView);
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext) override;
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = nullptr) override;
};