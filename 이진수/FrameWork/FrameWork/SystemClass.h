#pragma once
#ifndef _SYSTEMCLASS_H_ 
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputClass.h" 
#include "GraphicsClass.h"



class CSystemClass
{
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	CInputClass* m_Input;
	CGraphicsClass* m_Graphics;

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

public:
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

public:
	CSystemClass();
	CSystemClass(const CSystemClass&);
	~CSystemClass();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static CSystemClass* ApplicationHandle = 0;

#endif