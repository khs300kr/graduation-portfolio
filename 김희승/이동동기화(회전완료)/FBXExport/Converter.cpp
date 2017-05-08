#include "Converter.h"
#include "library.h"


void Converter::Initialize(const string& fileName)
{
	m_FileName = fileName;

	m_pManager = FbxManager::Create();//1

	if (!m_pManager)
	{
		cout << "FBX Manager Create Fail!!!" << endl;
		exit(1);
	}
	else
		cout << "FBX Manager Create Success" << endl;

	FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);//2.
	m_pManager->SetIOSettings(ios);

	FbxString lPath = FbxGetApplicationDirectory();//3.
	m_pManager->LoadPluginsDirectory(lPath.Buffer());

	FbxImporter* lImporter = FbxImporter::Create(m_pManager, "");//4.

	string filePath = fileName;
	if (!lImporter->Initialize(filePath.c_str(), -1, m_pManager->GetIOSettings()))//5.
	{
		cout << "FbxImporter Initialize Fail!! " << endl;
		cout << "Error returned: " << lImporter->GetStatus().GetErrorString() << endl;
		exit(-1);
	}
	else
	{
		m_Stat = LOADED;

		cout << m_FileName << "File" << " Importing...." << endl;
		cout << endl;
	}


	m_pScene = FbxScene::Create(m_pManager, "FBX Scene");//6.

	lImporter->Import(m_pScene);//7.

	m_pExporter = new Exporter();//8.

	//Destroy
	lImporter->Destroy();

	if (m_Stat == LOADED)
		LoadFile();

}
void Converter::LoadFile()
{
	FbxAxisSystem SceneAxisSystem = m_pScene->GetGlobalSettings().GetAxisSystem();

	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);

	if (SceneAxisSystem != OurAxisSystem)
	{
		OurAxisSystem.ConvertScene(m_pScene);
	}
	FbxGeometryConverter lGeomConverter(m_pManager);
	bool isTrianglute = lGeomConverter.Triangulate(m_pScene, true);

	cout << "[Export]" << '\t';
	m_pExporter->SetAnimationNameArray(m_pScene);

	m_pExporter->LoadSkeleton(m_pScene->GetRootNode());
	m_pExporter->LoadNodeRecursive(m_pScene->GetRootNode());

	auto formatPoint = find(m_FileName.begin(), m_FileName.end(), '.');
	m_FileName.erase(formatPoint, m_FileName.end());
	string fName = m_FileName + ".data";


	m_pExporter->WriteAllData(fName);


}
void Converter::Destroy()
{
	if (m_pManager)		m_pManager->Destroy();
	if (m_pScene)		m_pScene->Destroy();
}
