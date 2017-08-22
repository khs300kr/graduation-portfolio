#pragma once
#include "UIMesh.h"
#include "UIShader.h"

class CUIObject
{
	CUIMesh					*m_pMesh = nullptr;
	CTexture				*m_pTexture = nullptr;

	POINT					m_ptStartPos;
	POINT					m_ptEndPos;
	bool					m_bActive = true;
	TextureTag				m_tagTexture;
	float					m_fOpacity = 1.0f;

	

public:
	ID3D11Device*			pUIDevice;


	//CUIObject(ID3D11Device* , TextureTag);
	CUIObject();
	CUIObject(ID3D11Device*);
	virtual ~CUIObject();

	void Initialize(ID3D11Device* pDevice, POINT startPos, POINT endPos, float zPos);
	//	bool IsCollision(POINT mousePos);
	void SetEndPos(float endPos) { m_ptEndPos.x = endPos; }
	float GetEndPos() { return m_ptEndPos.x; }

	void Render(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	bool GetActive() const { return m_bActive; }
	TextureTag GetTag() const { return m_tagTexture; }

	void SetMaterial(CTexture* pMaterial) { m_pTexture = pMaterial; }
	void SetActive(bool set) { m_bActive = set; }

	float GetOpacity() const { return m_fOpacity; }


	void SetDevice(ID3D11Device* pDevice) { pUIDevice = pDevice; }
	

	void Update();
	void SetScore(POINT _sPoint, POINT _ePoint);
	void SetGamer(POINT _sPoint, POINT _ePoint);
};

class CUIManager
{
	CUIShader				*m_pUIShader = nullptr;
	CUIObject*				m_pBackGroundUI = nullptr;
	vector<CUIObject*>		m_vecUIObject;
	float					m_fSettingOpacity = 1.0f;

	ID3D11BlendState	*g_pTransparentBS;



public:
	CUIManager();
	virtual ~CUIManager();

	void Initialize(ID3D11Device* pDevice);

	TextureTag FindCollisionUIObject(POINT mousePos);
	void AddUIObject(CUIObject* object) { m_vecUIObject.push_back(object); };
	void RenderAll(ID3D11DeviceContext* pDeviceContext);

	// ----- Get, Setter ----- //
	CUIObject* GetUIObject(TextureTag tag);

	void SetBackGroundUI(CUIObject* ui) { m_pBackGroundUI = ui; }

};