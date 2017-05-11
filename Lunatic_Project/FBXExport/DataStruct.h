#pragma once

#include "library.h"

struct KeyVertex
{
	int				index;
	float			weight;
};

class VertexData
{
public:
	VertexData();
	VertexData(D3DXVECTOR3 dxvPos, D3DXVECTOR3 dxvNormal, D3DXVECTOR2 uv);
	VertexData(D3DXVECTOR3 dxvPos, D3DXVECTOR3 dxvNormal, D3DXVECTOR2 uv, D3DXVECTOR4 Weight, D3DXVECTOR4 Indices);
	~VertexData();
	BOOL operator == (CONST VertexData& others) const;
	BOOL operator != (CONST D3DXVECTOR3& others) const;

	D3DXVECTOR3		pos;
	D3DXVECTOR3		normal;
	D3DXVECTOR2		uv;
	D3DXVECTOR4		Weight;
	D3DXVECTOR4		Indices;
};

class MeshData
{
private:
	bool				m_hasNormal;
	bool				m_hasUv;
	bool				m_ByControlPoint;

	unsigned int		m_nIndices;
	unsigned int		m_nVertices;

	vector<VertexData>	m_VertexContainer;
	vector<UINT>		m_IndexContainer;

public:
	MeshData()
		:m_hasNormal(false),
		m_hasUv(false),
		m_ByControlPoint(true),
		m_nIndices(0),
		m_nVertices(0) {}
	~MeshData() {}

	std::vector<VertexData>&	GetMeshData() { return m_VertexContainer; }
	std::vector<UINT>&			GetIndexData() { return m_IndexContainer; }

	int							GetVertexCount() const { return m_nVertices; }
	int							GetIndexCount() const { return m_nIndices; }

	bool						GetNormal() const{ return m_hasNormal; }
	bool						GetUV() const { return m_hasUv; }
	bool						GetByControlPoint() const { return m_ByControlPoint; }

	void						SetVertexCount(unsigned int count) { m_nVertices = count; }
	void						SetIndexCount(unsigned int count) { m_nIndices = count; }

	void						SetNormal(bool hasNormal) { m_hasNormal = hasNormal; }
	void						SetUV(bool hasUV) { m_hasUv = hasUV; }
	void						SetByControlPoint(bool byControlPoint) { m_ByControlPoint = byControlPoint; }
};

class KeyFrameData
{
public:
	FbxAMatrix					m_fTransformMtx;
	double						m_fTime;

	KeyFrameData();
	~KeyFrameData();
};

class BoneData
{
private:
	vector<KeyFrameData>		m_bAnim;
	string						m_bName;
	unsigned int				m_bParentIndex;
	FbxAMatrix					m_bOffsetMtx;

public:
	BoneData();
	~BoneData();

	void								SetParentIndex(int index) { m_bParentIndex = index; }
	void								SetBoneName(const std::string& name) { m_bName = name; }
	void								SetOffsetMatrix(const FbxAMatrix& mtx) { m_bOffsetMtx = mtx; }

	int									GetParentIndex() const { return m_bParentIndex; }
	std::string							GetBoneName() const { return m_bName; }
	FbxAMatrix							GetOffsetMatrix() const { return m_bOffsetMtx; }

	std::vector<KeyFrameData>&			GetBoneAnimation() { return m_bAnim; }
	BOOL operator == (CONST BoneData& others) const
	{
		return this->m_bName == others.m_bName;
	}
	BOOL operator != (CONST BoneData& others) const
	{
		return this->m_bName != others.m_bName;
	}
};
//FbxAnimStack* currAnimStack = m_FBXScene->GetSrcObject<FbxAnimStack>(0);
//FbxString animStackName = currAnimStack->GetName();
class SkinnedData
{
public:
	typedef std::vector<KeyVertex>		KeyVertexContainer;

private:
	unsigned int						m_sBoneCnt;
	std::string							m_sName;

	FbxTime								m_tStart;
	FbxTime								m_tEnd;
	FbxLongLong							m_tAnimLength;

	std::map<int, KeyVertexContainer>	m_mapControlPoint;
	std::vector<BoneData>				m_SkeletonContainer;


public:
	SkinnedData();
	~SkinnedData();

	void								SetBoneCount(int count) { m_sBoneCnt = count; }
	void								SetAnimationName(const std::string& name) { m_sName = name; }
	void								SetAnimationLength(FbxLongLong length) { m_tAnimLength = length; }
	void								SetTimeStart(FbxTime time) { m_tStart = time; }
	void								SetTimeEnd(FbxTime time) { m_tEnd = time; }

	FbxLongLong							GetTimeStart() const { return m_tStart.GetFrameCount(FbxTime::eFrames30); }
	FbxLongLong							GetTimeEnd() const { return m_tEnd.GetFrameCount(FbxTime::eFrames30); }
	int									GetBoneCount() const { return m_sBoneCnt; }
	std::string							GetAnimationName() const { return m_sName; }
	FbxLongLong							GetAnimationLength() const { return m_tAnimLength; }

	std::map<int, KeyVertexContainer>&	GetControlPoint() { return m_mapControlPoint; }
	std::vector<BoneData>&				GetSkeleton() { return m_SkeletonContainer; }
};