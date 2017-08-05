#pragma once
#include "Mesh.h"
class CUIMesh :	public CMeshTextured
{
public:
	CUIMesh(ID3D11Device *pd3dDevice, POINT startPos, POINT endPos, float zPos);
	virtual ~CUIMesh();
};

