#pragma once
class CInputClass
{
private:
	bool m_keys[256];

public:
	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

public:
	CInputClass();
	CInputClass(const CInputClass&);
	~CInputClass();
};

