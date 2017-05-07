#pragma once

#include "Exporter.h"

class Converter
{
private:
	string				m_FileName;
	int					m_Stat;
	
	FbxScene*			m_pScene;
	FbxManager*			m_pManager;

	Exporter*			m_pExporter;

	Converter() : m_pManager(nullptr), m_pScene(nullptr), m_Stat(UNLOADED), m_pExporter(nullptr){}
	Converter(const Converter&){}
	Converter& operator = (const Converter&)const{}
	~Converter(){}

public:
	static Converter* Instance()
	{
		static Converter instance;
		return &instance;
	}

	void Initialize(const string& fileName);
	void LoadFile();
	void Destroy();

	FbxScene* GetScene() const { return m_pScene; }
};