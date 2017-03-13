#pragma once

#include "D3DClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class CGraphicsClass
{
private:
	CD3DClass* m_D3D;

private:
	bool Render();

public:
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

public:
	CGraphicsClass();
	CGraphicsClass(const CGraphicsClass&);
	~CGraphicsClass();
};

