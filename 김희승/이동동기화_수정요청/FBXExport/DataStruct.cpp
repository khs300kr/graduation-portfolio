#include "library.h"
#include "DataStruct.h"

VertexData::VertexData()
: pos(0, 0, 0), normal(0, 0, 0), uv(0, 0), Weight(0, 0, 0, 0), Indices(0, 0, 0, 0)
{
}
VertexData::VertexData(D3DXVECTOR3 dxvPos, D3DXVECTOR3 dxvNormal, D3DXVECTOR2 uv)
: pos(dxvPos), normal(dxvNormal), uv(uv), Weight(0, 0, 0, 0), Indices(0, 0, 0, 0)
{
}
VertexData::VertexData(D3DXVECTOR3 dxvPos, D3DXVECTOR3 dxvNormal, D3DXVECTOR2 uv, D3DXVECTOR4 Weight, D3DXVECTOR4 Indices)
: pos(dxvPos), normal(dxvNormal), uv(uv), Weight(Weight), Indices(Indices)
{
}
VertexData::~VertexData()
{
}
BOOL VertexData::operator == (CONST VertexData& others) const
{
	float diff_P_X = 0, diff_P_Y = 0, diff_P_Z = 0;
	float diff_N_X = 0, diff_N_Y = 0, diff_N_Z = 0;
	float diff_T_X = 0, diff_T_Y = 0;

	diff_P_X = this->pos.x - others.pos.x;
	diff_P_Y = this->pos.y - others.pos.y;
	diff_P_Z = this->pos.z - others.pos.z;

	diff_N_X = this->normal.x - others.normal.x;
	diff_N_Y = this->normal.y - others.normal.y;
	diff_N_Z = this->normal.z - others.normal.z;

	diff_T_X = this->uv.x - others.uv.x;
	diff_T_Y = this->uv.y - others.uv.y;


	bool comparePos = (fabs(diff_P_X) <= 0.0001) && (fabs(diff_P_Y) <= 0.0001) && (fabs(diff_P_Z) <= 0.0001);
	bool compareNormal = (fabs(diff_N_X) <= 0.0001) && (fabs(diff_N_Y) <= 0.0001) && (fabs(diff_N_Z) <= 0.0001);
	bool compareUV = (fabs(diff_T_X) <= 0.0001) && (fabs(diff_T_Y) <= 0.0001);

	if (comparePos && compareNormal && compareUV) 
		return true;
	else										   
		return false;
}


KeyFrameData::KeyFrameData()
: m_fTime (0)
{
}
KeyFrameData::~KeyFrameData()
{
}


BoneData::BoneData()
: m_bParentIndex(0)
{
}
BoneData::~BoneData()
{
}


SkinnedData::SkinnedData()
:m_tAnimLength(0), m_sBoneCnt(0)
{
}
SkinnedData::~SkinnedData()
{
}