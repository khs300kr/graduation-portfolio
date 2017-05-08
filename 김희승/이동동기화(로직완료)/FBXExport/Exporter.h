#pragma once
#include "DataStruct.h"

class Exporter
{
private:
	vector<MeshData>				 m_exMeshContainer;
	vector<MeshData>::iterator		 m_pMeshIterator;

	vector<SkinnedData>				 m_exSkinnedContainer;
	vector<SkinnedData>::iterator	 m_pSkinnedIterator;

	FbxArray<FbxString*>			 m_sAnimationNames;

	unsigned int					 m_iMeshCount;
	bool							 mHasAnimationData;
public:
	Exporter();
	~Exporter();

public:
	void							SetMeshCount();
	void							SetMeshCount(unsigned int count) { m_iMeshCount = count; }
	void							SetAnimationNameArray(FbxScene* pScene);

	MeshData&						GetIndexMeshData(int index);
	SkinnedData&					GetIndexSkinnedData(int index);
	const FbxArray<FbxString*>&		GetAnimationNameArray() const { return m_sAnimationNames; }

	void							LoadNodeRecursive(FbxNode* pNode);
	void							LoadSkeleton(FbxNode* pNode);

	int								FindBoneIndexUsingName(const string& boneName, int meshIndex);

	bool							ExportSkinnedData(FbxMesh* pMesh);
	bool							ExportMeshData(FbxMesh* pMesh);

	void							ExportBoneHierarchy(FbxNode* currNode, int myIndex, int parentIndex);
	void							ExportBoneOffsetMatrix(FbxCluster* pCluster, FbxAMatrix& globalPosition, int currBoneIndex);
	void							ExportkeyVertexInfo(FbxCluster* pCluster, int weightIndicesCount, int currBoneIndex);
	bool							ExportAnimationNameAndLength(int index);
	void							ExportTimeTransformMatrix(FbxNode* pNode, FbxCluster* pCluster, FbxAMatrix& globalPosition, int currBoneIndex);
	void							TransferkeyVertexInfo(FbxMesh* pMesh);

	void							WriteVertexData(ofstream& fout, int meshID);
	void							WriteSkinnedVertexData(ofstream& fout, int meshID);
	void							WriteIndexData(ofstream& fout, int meshID);
	void							WriteBoneHierarchy(ofstream& fout, int meshID);
	void							WriteOffsetMatrix(ofstream& fout, int meshID);
	void							WriteTimeTransformMatrix(ofstream& fout, int meshID);
	void							WriteConvertTransformMatrix(ofstream& fout, KeyFrameData& mtx);
	void							WriteAllData(const string& fileName);

};


