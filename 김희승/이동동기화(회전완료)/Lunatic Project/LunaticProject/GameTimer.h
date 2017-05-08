#pragma once

const ULONG MAX_SAMPLE_COUNT = 50; // 50ȸ�� ������ ó�� �ð��� �����Ͽ� ��� ó��

class CGameTimer
{
private:
	HRESULT m_hrHardwareHasPerformanceCounter;
	float m_fTimeScale;
	float m_fTimeElapsed;
	__int64 m_nCurrentTime;
	__int64 m_nLastTime;
	__int64 m_nPerformanceFrequency;

	float m_fFrameTime[MAX_SAMPLE_COUNT];
	ULONG m_nSampleCount;

	unsigned long m_nCurrentFrameRate;
	unsigned long m_FramePerSecond;
	float m_fFPSTimeElapsed;

public:
	CGameTimer();
	~CGameTimer();

	void Tick(float fLockFPS = 0.0f);
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetTimeElapsed();
};

