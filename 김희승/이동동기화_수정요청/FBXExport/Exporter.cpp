#include "library.h"
#include "Exporter.h"
#include "GetPosition.h"
#include "Converter.h"


Exporter::Exporter()
	: m_iMeshCount(0), mHasAnimationData(false)
{
	m_exSkinnedContainer.resize(2);
}


Exporter::~Exporter()
{
}
void Exporter::SetMeshCount()
{

}
void Exporter::SetAnimationNameArray(FbxScene* pScene)
{
	pScene->FillAnimStackNameArray(m_sAnimationNames);
}
void Exporter::LoadNodeRecursive(FbxNode* pNode)
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	if (pNodeAttribute)
	{
		if (pNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = pNode->GetMesh();
			if (pMesh)
			{
				if (ExportMeshData(pMesh))
					cout << "[Mesh Data Export Success]" << endl;

				else
					cout << "[Mesh Data Export Fail]" << endl;

			}
		}

	}
	const int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)	//Recursive
		LoadNodeRecursive(pNode->GetChild(i));
}
void Exporter::LoadSkeleton(FbxNode* pNode)
{
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
		ExportBoneHierarchy(pNode->GetChild(childIndex), 0, -1);

}
bool Exporter::ExportMeshData(FbxMesh* pMesh)
{
	if (!pMesh->GetNode()) return false;

	cout << endl << "[Mesh Data Exporting...]" << endl;

	MeshData vMeshData;

	FbxLayerElementArrayTemplate<int>* pMaterialIndices = NULL;
	FbxGeometryElementNormal* vertexNormal = nullptr;
	FbxGeometryElementUV* vertexUV = nullptr;
	FbxGeometryElement::EMappingMode eNormalMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode eUVMappingMode = FbxGeometryElement::eNone;
	FbxGeometryElement::EReferenceMode eNormalReferenceMode;

	vMeshData.SetNormal(pMesh->GetElementNormalCount() > 0);
	vMeshData.SetUV(pMesh->GetElementUVCount() > 0);

	if (vMeshData.GetNormal())
	{
		vertexNormal = pMesh->GetElementNormal(0);
		eNormalMappingMode = vertexNormal->GetMappingMode();
		eNormalReferenceMode = vertexNormal->GetReferenceMode();
		if (eNormalMappingMode == FbxGeometryElement::eNone)
		{
			vMeshData.SetNormal(false);
		}
		if (vMeshData.GetNormal() && eNormalMappingMode != FbxGeometryElement::eByControlPoint)
		{
			vMeshData.SetByControlPoint(false);
		}
	}
	if (vMeshData.GetUV())	//uv?																					
	{
		eUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
		if (eUVMappingMode == FbxGeometryElement::eNone)
		{
			vMeshData.SetUV(false);
		}
		if (vMeshData.GetUV() && eUVMappingMode != FbxGeometryElement::eByControlPoint)
		{
			vMeshData.SetByControlPoint(false);
		}
	}

	const int polygonCount = pMesh->GetPolygonCount();
	int polygonVertexCount = 0;

	polygonVertexCount = pMesh->GetPolygonVertexCount();

	if (vMeshData.GetByControlPoint())
		polygonVertexCount = pMesh->GetControlPointsCount();

	cout << "Polygon Count : " << polygonCount << endl;
	cout << "Polygon Vertex Count : " << polygonVertexCount << endl;

	vMeshData.GetMeshData().reserve(polygonVertexCount);
	vMeshData.GetIndexData().resize(polygonCount * 3);

	const FbxVector4* pControlPoints = pMesh->GetControlPoints();
	FbxVector4 currVertex;
	FbxVector4 currNormal;
	FbxVector2 currUV;

	FbxStringList UVNames;
	pMesh->GetUVSetNames(UVNames);
	const char* pUVName = nullptr;
	if (vMeshData.GetUV() && UVNames.GetCount())
		pUVName = UVNames[0];


	int vertexCounter = 0;
	int indexCounter = 0;

	if (!vMeshData.GetByControlPoint())
	{
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
		{
			VertexData currVertexData;

			for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
			{
				const int controlPointIndex = pMesh->GetPolygonVertex(polygonIndex, vertexIndex);

				currVertex = pControlPoints[controlPointIndex];
				currVertexData.pos.x = static_cast<float>(currVertex[0]);
				currVertexData.pos.y = static_cast<float>(currVertex[1]);
				currVertexData.pos.z = static_cast<float>(currVertex[2]);

				if (vMeshData.GetNormal())
				{
					int index = 0;

					switch (eNormalReferenceMode)
					{
					case FbxGeometryElement::eDirect:
						pMesh->GetPolygonVertexNormal(polygonIndex, vertexIndex, currNormal);
						currVertexData.normal.x = static_cast<float>(currNormal[0]);
						currVertexData.normal.y = static_cast<float>(currNormal[1]);
						currVertexData.normal.z = static_cast<float>(currNormal[2]);


						break;

					case FbxGeometryElement::eIndexToDirect:
						index = vertexNormal->GetIndexArray().GetAt(indexCounter);

						currVertexData.normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
						currVertexData.normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
						currVertexData.normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);

						break;

					default:
						throw std::exception("잘못된 참조입니다.");
					}

				}
				if (vMeshData.GetUV())
				{
					bool UnmappedUV;
					pMesh->GetPolygonVertexUV(polygonIndex, vertexIndex, pUVName, currUV, UnmappedUV);
					currVertexData.uv.x = static_cast<float>(currUV[0]);
					currVertexData.uv.y = 1.0f - static_cast<float>(currUV[1]);
				}


				auto endPoint = vMeshData.GetMeshData().end();
				auto searchPoint = find(vMeshData.GetMeshData().begin(), vMeshData.GetMeshData().end(), currVertexData);

				// 중복 발생하지 않을경우.
				if (endPoint == searchPoint)
				{
					vMeshData.GetIndexData()[indexCounter] = vertexCounter;
					vMeshData.GetMeshData().push_back(currVertexData);
					vertexCounter++;
				}
				// 중복이 발생할 경우.
				else
				{
					int indexNumber = distance(vMeshData.GetMeshData().begin(), searchPoint);
					vMeshData.GetIndexData()[indexCounter] = indexNumber;
				}
				indexCounter++;

			}/// Vertex Loop

		}/// Polygon Loop

		vMeshData.SetIndexCount(indexCounter);
		vMeshData.SetVertexCount(vertexCounter);
	}
	cout << endl;


	cout << "[Export Success]" << endl;
	cout << "Vertex Count : " << vMeshData.GetVertexCount() << endl;
	cout << "Index Count : " << vMeshData.GetIndexCount() << endl;
	cout << endl;

	m_exMeshContainer.push_back(vMeshData);


	const bool lHasVertexCache = pMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(pMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
	const bool lHasShape = pMesh->GetShapeCount() > 0;
	const bool lHasSkin = pMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasSkin;

	mHasAnimationData = lHasDeformation;

	if (lHasDeformation)
	{
		if (ExportAnimationNameAndLength(m_iMeshCount))
			cout << "[Animation Meta Data Export Success]" << endl;
		else
			cout << "[Animation Meta Data Export Fail]" << endl;

		if (ExportSkinnedData(pMesh))
			cout << "[Animation Data Export Success]" << endl;
		else
			cout << "[Animation Data Export Fail]" << endl;
	}

	m_iMeshCount++;

	return true;
}

bool Exporter::ExportSkinnedData(FbxMesh* pMesh)
{
	FbxSkin* lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	if (lSkinningType != FbxSkin::eRigid && lSkinningType != FbxSkin::eLinear)
		return false;

	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	int lControlPointsCount = pMesh->GetControlPointsCount();

	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
	{
		FbxSkin* lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

		int lClusterCount = lSkinDeformer->GetClusterCount();

		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);

			if (!lCluster->GetLink())
				continue;

			string boneName = lCluster->GetLink()->GetName();
			int boneIndex = FindBoneIndexUsingName(boneName, m_iMeshCount);
			FbxAMatrix geometryTransform = GetGeometry(pMesh->GetNode());

			ExportkeyVertexInfo(lCluster, lCluster->GetControlPointIndicesCount(), boneIndex);
			ExportBoneOffsetMatrix(lCluster, geometryTransform, boneIndex);
			ExportTimeTransformMatrix(pMesh->GetNode(), lCluster, geometryTransform, boneIndex);
		}

	}
	TransferkeyVertexInfo(pMesh);

	return true;

}
void Exporter::ExportBoneOffsetMatrix(FbxCluster* pCluster, FbxAMatrix& globalPosition, int currBoneIndex)
{
	FbxAMatrix transformMatrix;
	FbxAMatrix transformLinkMatrix;
	FbxAMatrix offsetMatrix;
	FbxAMatrix reflectionMatrix;
	FbxAMatrix finalOffsetMatrix;

	pCluster->GetTransformMatrix(transformMatrix);
	pCluster->GetTransformLinkMatrix(transformLinkMatrix);

	// 반사행렬 생성
	FbxDouble4 row_1 = { 1, 0, 0, 0 };
	FbxDouble4 row_2 = { 0, 0, 1, 0 };
	FbxDouble4 row_3 = { 0, 1, 0, 0 };
	FbxDouble4 row_4 = { 0, 0, 0, 1 };
	reflectionMatrix.mData[0] = row_1;
	reflectionMatrix.mData[1] = row_2;
	reflectionMatrix.mData[2] = row_3;
	reflectionMatrix.mData[3] = row_4;

	// OffSet Matrix를 구한다.
	offsetMatrix = transformLinkMatrix.Inverse() * transformMatrix * globalPosition;

	// 반사행렬을 양쪽으로 곱해준다.
	finalOffsetMatrix = reflectionMatrix * offsetMatrix * reflectionMatrix;

	auto skinnedIter = m_exSkinnedContainer.begin() + m_iMeshCount;

	skinnedIter->GetSkeleton()[currBoneIndex].SetOffsetMatrix(finalOffsetMatrix);

}
bool Exporter::ExportAnimationNameAndLength(int index)
{
	FbxAnimStack* currAnimStack = Converter::Instance()->GetScene()->FindMember<FbxAnimStack>(m_sAnimationNames[index]->Buffer());
	if (currAnimStack == NULL)
	{
		return false;
	}
	FbxString animStackName = currAnimStack->GetName();

	m_exSkinnedContainer[index].SetAnimationName(animStackName.Buffer());

	FbxTakeInfo* takeInfo = Converter::Instance()->GetScene()->GetTakeInfo(animStackName);

	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	FbxTime::EMode timeMode = Converter::Instance()->GetScene()->GetGlobalSettings().GetTimeMode();

	m_exSkinnedContainer[index].SetTimeStart(start);
	m_exSkinnedContainer[index].SetTimeEnd(end);
	m_exSkinnedContainer[index].SetAnimationLength(end.GetFrameCount(timeMode) - start.GetFrameCount(timeMode) + 1);

	return true;
}
void Exporter::ExportTimeTransformMatrix(FbxNode* pNode, FbxCluster* pCluster, FbxAMatrix& globalPosition, int currBoneIndex)
{
	m_pSkinnedIterator = m_exSkinnedContainer.begin() + m_iMeshCount;

	vector<KeyFrameData>& boneAnimations = m_pSkinnedIterator->GetSkeleton()[currBoneIndex].GetBoneAnimation();
	FbxTime::EMode timeMode = Converter::Instance()->GetScene()->GetGlobalSettings().GetTimeMode();

	FbxAMatrix reflectionMtx;

	// 반사행렬 생성
	FbxDouble4 row_1 = { 1, 0, 0, 0 };
	FbxDouble4 row_2 = { 0, 0, 1, 0 };
	FbxDouble4 row_3 = { 0, 1, 0, 0 };
	FbxDouble4 row_4 = { 0, 0, 0, 1 };
	reflectionMtx.mData[0] = row_1;
	reflectionMtx.mData[1] = row_2;
	reflectionMtx.mData[2] = row_3;
	reflectionMtx.mData[3] = row_4;

	for (FbxLongLong timePos = m_pSkinnedIterator->GetTimeStart(); timePos <= m_pSkinnedIterator->GetTimeEnd(); ++timePos)
	{
		KeyFrameData currFrame;
		FbxTime currTime;
		currTime.SetFrame(timePos, timeMode);

		FbxAMatrix currTransformOffsetMtx = pNode->EvaluateGlobalTransform(currTime) * globalPosition;

		// time t transform mtx
		FbxAMatrix currTransformMtx = currTransformOffsetMtx.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(currTime);

		// 최종 행렬 = 반사 행렬 * time t transform mtx * 반사행렬
		FbxAMatrix finalTransformMtx = reflectionMtx * currTransformMtx * reflectionMtx;

		currFrame.m_fTime = currTime.GetSecondDouble();
		currFrame.m_fTransformMtx = finalTransformMtx;

		boneAnimations.push_back(currFrame);

	}

}
void Exporter::ExportkeyVertexInfo(FbxCluster* pCluster, int weightIndicesCount, int currBoneIndex)
{
	int currClusterIndex = 0;

	for (int influenceIndex = 0; influenceIndex < weightIndicesCount; ++influenceIndex)
	{
		KeyVertex currkeyVertexInfo;
		currkeyVertexInfo.index = currBoneIndex;
		currkeyVertexInfo.weight = pCluster->GetControlPointWeights()[influenceIndex];

		currClusterIndex = pCluster->GetControlPointIndices()[influenceIndex];
		m_exSkinnedContainer[m_iMeshCount].GetControlPoint()[currClusterIndex].push_back(currkeyVertexInfo);

	}


}

void Exporter::ExportBoneHierarchy(FbxNode* currNode, int myIndex, int parentIndex)
{
	if (currNode->GetNodeAttribute() && currNode->GetNodeAttribute()->GetAttributeType() && currNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		BoneData currBone;
		currBone.SetParentIndex(parentIndex);
		currBone.SetBoneName(currNode->GetName());
		m_exSkinnedContainer[0].GetSkeleton().push_back(currBone);
	}
	for (int i = 0; i < currNode->GetChildCount(); ++i)
	{
		ExportBoneHierarchy(currNode->GetChild(i), m_exSkinnedContainer[0].GetSkeleton().size(), myIndex);
	}
}
int Exporter::FindBoneIndexUsingName(const string& boneName, int meshIndex)
{
	m_pSkinnedIterator = m_exSkinnedContainer.begin() + meshIndex;

	int boneIndex = 0;

	auto searchIter = m_pSkinnedIterator->GetSkeleton().begin();
	auto endPoint = m_pSkinnedIterator->GetSkeleton().end();

	for (; searchIter != endPoint; ++searchIter)
	{
		if (boneName == searchIter->GetBoneName())
			return boneIndex;

		boneIndex++;
	}

	return -2;


}
void Exporter::TransferkeyVertexInfo(FbxMesh* pMesh)
{
	m_pSkinnedIterator = m_exSkinnedContainer.begin() + m_iMeshCount;
	m_pMeshIterator = m_exMeshContainer.begin() + m_iMeshCount;

	int influenceInfoSize = m_pSkinnedIterator->GetControlPoint().size();

	if (influenceInfoSize == pMesh->GetControlPointsCount())
	{
		map<int, SkinnedData::KeyVertexContainer>& collMap = m_pSkinnedIterator->GetControlPoint();
		vector<VertexData>& collVector = m_pMeshIterator->GetMeshData();

		const FbxVector4* pControlPoints = pMesh->GetControlPoints();
		FbxVector4 currVertex;

		for (auto& keyVertex : collMap)
		{
			currVertex = pControlPoints[keyVertex.first];

			D3DXVECTOR3 influencePos;
			influencePos.x = static_cast<float>(currVertex[0]);
			influencePos.y = static_cast<float>(currVertex[1]);
			influencePos.z = static_cast<float>(currVertex[2]);

			for (auto& vertex : collVector)
			{
				if (vertex.pos == influencePos)
				{
					int boneIndices[4] = { 0, 0, 0, 0 };
					float boneWeights[4] = { 0.0, 0.0, 0.0, 0.0 };


					if (keyVertex.second.size() > 4)
					{
						//내림차순 
						sort(keyVertex.second.begin(), keyVertex.second.end(), [](const KeyVertex& lhs, const KeyVertex& rhs)
						{
							return lhs.weight > rhs.weight;
						});

						//2.
						float sumWeight = 0.0f, interpolateWeight = 0.0f;
						for (int i = 0; i < 4; ++i)
						{
							sumWeight += keyVertex.second[i].weight;
						}
						interpolateWeight = 1.0 - sumWeight;
						//3.
						auto removeIter = keyVertex.second.begin() + 4;
						keyVertex.second.erase(removeIter, keyVertex.second.end());
						//4.
						keyVertex.second[0].weight += interpolateWeight;

					}

					int influenceCount = 0;
					for (auto influenceInfo : keyVertex.second)
					{

						boneIndices[influenceCount] = influenceInfo.index;
						boneWeights[influenceCount] = influenceInfo.weight;

						influenceCount++;
					}

					vertex.Indices.x = (float)boneIndices[0];
					vertex.Indices.y = (float)boneIndices[1];
					vertex.Indices.z = (float)boneIndices[2];
					vertex.Indices.w = (float)boneIndices[3];

					vertex.Weight.x = boneWeights[0];
					vertex.Weight.y = boneWeights[1];
					vertex.Weight.z = boneWeights[2];
					vertex.Weight.w = boneWeights[3];

				}
			}
		}
	}

}
MeshData& Exporter::GetIndexMeshData(int index)
{
	if (m_exMeshContainer.size() >= index)
		m_pMeshIterator = m_exMeshContainer.begin() + index;
	else
		cout << "Input Index Data Error" << endl;

	return *m_pMeshIterator;


}
SkinnedData& Exporter::GetIndexSkinnedData(int index)
{
	if (m_exSkinnedContainer.size() >= index)
		m_pSkinnedIterator = m_exSkinnedContainer.begin() + index;
	else
		cout << "Input Index Data Error" << endl;

	return *m_pSkinnedIterator;
}

void Exporter::WriteVertexData(ofstream& fout, int meshID)
{
	for (auto vertex : m_exMeshContainer[meshID].GetMeshData())
	{
		fout << vertex.pos.x << ' ' << vertex.pos.z << ' ' << vertex.pos.y << endl
			 << vertex.normal.x << ' ' << vertex.normal.z << ' ' << vertex.normal.y << endl
			<< vertex.uv.x << ' ' << vertex.uv.y << endl;
	}
}
void Exporter::WriteSkinnedVertexData(ofstream& fout, int meshID)
{
	for (auto vertex : m_exMeshContainer[meshID].GetMeshData())
	{
		//fout << fixed;
		//fout.precision(6);
		fout << vertex.pos.x << ' ' << vertex.pos.z << ' ' << vertex.pos.y << endl
			<< vertex.normal.x << ' ' << vertex.normal.z << ' ' << vertex.normal.y << endl
			<< vertex.uv.x << ' ' << vertex.uv.y << endl
			<< (int)vertex.Indices.x << ' ' << (int)vertex.Indices.y << ' ' << (int)vertex.Indices.z << ' ' << (int)vertex.Indices.w << endl
			<< vertex.Weight.x << ' ' << vertex.Weight.y << ' ' << vertex.Weight.z << ' ' << vertex.Weight.w << endl;
	}
}
void Exporter::WriteIndexData(ofstream& fout, int meshID)
{
	int cnt = 1;

	int triangleCount = m_exMeshContainer[meshID].GetIndexCount() / 3;
	for (int i = 0; i < triangleCount; ++i)
	{
		fout << m_exMeshContainer[meshID].GetIndexData()[i * 3] << ' '
			<< m_exMeshContainer[meshID].GetIndexData()[i * 3 + 2] << ' '
			<< m_exMeshContainer[meshID].GetIndexData()[i * 3 + 1]
			<< endl;
	}

}
void Exporter::WriteOffsetMatrix(ofstream& fout, int meshID)
{
	int cnt = 0;

	for (auto bone : m_exSkinnedContainer[meshID].GetSkeleton())
	{
		FbxAMatrix mtx = bone.GetOffsetMatrix();

		fout << to_string(cnt)
			<< mtx.Get(0, 0) << ' ' << mtx.Get(0, 1) << ' ' << mtx.Get(0, 2) << ' ' << mtx.Get(0, 3) << ' '
			<< mtx.Get(1, 0) << ' ' << mtx.Get(1, 1) << ' ' << mtx.Get(1, 2) << ' ' << mtx.Get(1, 3) << ' '
			<< mtx.Get(2, 0) << ' ' << mtx.Get(2, 1) << ' ' << mtx.Get(2, 2) << ' ' << mtx.Get(2, 3) << ' '
			<< mtx.Get(3, 0) << ' ' << mtx.Get(3, 1) << ' ' << mtx.Get(3, 2) << ' ' << mtx.Get(3, 3) << ' '
			<< endl;

		cnt++;
	}

}
void Exporter::WriteConvertTransformMatrix(ofstream& fout, KeyFrameData& frame)
{
	FbxAMatrix mtx = frame.m_fTransformMtx;
	FbxVector4 T, S;
	FbxQuaternion Q;

	T = mtx.GetT();
	S = mtx.GetS();
	Q = mtx.GetQ();

	fout << frame.m_fTime << ' '
		<<  T.mData[0] << ' ' << T.mData[1] << ' ' << T.mData[2] << ' '
		<<  S.mData[0] << ' ' << S.mData[1] << ' ' << S.mData[2] << ' '
		<<  Q.mData[0] << ' ' << Q.mData[1] << ' ' << Q.mData[2] << ' ' << Q.mData[3]
		<< endl;



}
void Exporter::WriteTimeTransformMatrix(ofstream& fout, int meshID)
{
	string replaceName = m_exSkinnedContainer[meshID].GetAnimationName();
	replace(replaceName.begin(), replaceName.end(), ' ', '_');
	fout << replaceName << endl;
	

	int boneCnt = 0;
	for (auto bone : m_exSkinnedContainer[meshID].GetSkeleton())
	{
		fout << to_string(boneCnt) <<' '<< bone.GetBoneAnimation().size() << endl;
		//fout << '\t' << '{' << endl;

		for (auto frame : bone.GetBoneAnimation())
		{
			//fout << '\t' << '\t';
			WriteConvertTransformMatrix(fout, frame); // TSQ 저장
		}
		//fout << endl;

		boneCnt++;
	}
	


}
void Exporter::WriteBoneHierarchy(ofstream& fout, int meshID)
{
	int cnt = 0;

	for (auto bone : m_exSkinnedContainer[meshID].GetSkeleton())
	{
		fout << bone.GetParentIndex() << endl;
		cnt++;
	}
}

void Exporter::WriteAllData(const string& fileName)
{
	ofstream fout(fileName);

	if (!fout)
	{
		cerr << fileName << "File Load Error!" << endl;
		exit(EXIT_FAILURE);
	}

	//fout << "[ FBX_META_DATA ]" << endl;
	//fout << "MeshCount: " << m_iMeshCount << endl;
	//fout << endl;
	for (int i = 0; i < m_iMeshCount; ++i)
	{
		/// Mesh Data Export
		cout << "[" << i + 1 << "]-> " << " Mesh Data Recording ..." << endl;

		fout << "[ MESH_DATA ]" << endl;
		fout << GetIndexMeshData(i).GetMeshData().size() << endl;
		fout << GetIndexMeshData(i).GetIndexCount() << endl;
		fout << GetIndexSkinnedData(i).GetSkeleton().size() << endl;
		fout << Converter::Instance()->GetScene()->GetSrcObjectCount<FbxAnimStack>() << endl;

		fout << "[ VERTEX_DATA ]" << endl;

		if (mHasAnimationData)
			WriteSkinnedVertexData(fout, i);
		else
			WriteVertexData(fout, i);

		fout << "[ INDEX_DATA ]" << endl;
		WriteIndexData(fout, i);
		cout << "[" << i + 1 << "]-> " << " Mesh Data Success !!" << endl;
		if (mHasAnimationData)
		{
			/// BoneHierarchy Data Export									
			cout << "[" << i + 1 << "]-> " << " Bone Hierarchy Data Recording ..." << endl;

			fout << "[ BONE_HIERARCHY ]" << endl;

			WriteBoneHierarchy(fout, i);

			cout << "[" << i + 1 << "]-> " << " Bone Hierarchy  Data Success !!" << endl;


			/// OffsetMatrix Data Export 
			cout << "[" << i + 1 << "]-> " << " Offset Matrix Data Recording ..." << endl;

			fout << "[ OFFSET_MATRIX ]" << endl;

			WriteOffsetMatrix(fout, i);

			cout << "[" << i + 1 << "]-> " << " Offset Matrix Data Success !!" << endl;


			/// Time t TransformMatrix Data Export 
			cout << "[" << i + 1 << "]-> " << " Time t Transform Matrix Data Recording ..." << endl;

			fout << "[ ANIMATION_CLIPS ]: ";// << endl;

			WriteTimeTransformMatrix(fout, i);
			fout << endl;

			cout << "[" << i + 1 << "]-> " << " Time t Transform Matrix Data Success !!" << endl;
		}
	}

	fout.close();


}