/****************************************************************************************

   Copyright (C) 2014 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This file contains the functions to get the global 
// position of a node for a given time in the current animation stack.
//
/////////////////////////////////////////////////////////////////////////
#include "library.h"
#include "GetPosition.h"

// Get the global position of the node for the current pose.
// If the specified node is not part of the pose or no pose is specified, get its
// global position at the current time.
/// 현재 Pose를 위한 Node의 Global Position을 가져온다.
/// 만약 특정노드가 pose의 일부가 아니거나 pose가 지정되어 있지 않다면, 현재 시간의 노드의 Global Position을 가져온다.

FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
{
    FbxAMatrix lGlobalPosition;
    bool        lPositionFound = false;

    if (pPose)
    {
        int lNodeIndex = pPose->Find(pNode);

        if (lNodeIndex > -1)
        {
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
			/// Bind Pose는 항상 글로벌 행렬이다.
			/// 우리가 Rest Pose를 가진다면, 그것이 글로벌인지 로컬공간에 저장된것인지에 대한 검사가 필요하다. 
            if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
            {
                lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
            }
            else
            {
                // We have a local matrix, we need to convert it to
                // a global space matrix.
				/// 우리는 로컬 행렬을 가진다, 우리는 그것을 글로벌 공간 행렬로 Convert하는것이 필요하다.
                FbxAMatrix lParentGlobalPosition;

                if (pParentGlobalPosition)
                {
                    lParentGlobalPosition = *pParentGlobalPosition;
                }
                else
                {
                    if (pNode->GetParent())
                    {
                        lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
                    }
                }

                FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
                lGlobalPosition = lParentGlobalPosition * lLocalPosition;
            }

            lPositionFound = true;
        }
    }

    if (!lPositionFound)
    {
        // There is no pose entry for that node, get the current global position instead.

        // Ideally this would use parent global position and local position to compute the global position.
        // Unfortunately the equation 
        //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
        // does not hold when inheritance type is other than "Parent" (RSrs).
        // To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		/// 노드를 위한 pose entry가 아니다, 현재의 글로벌 포지션을 가져오는것으로 대신한다.
		/// 이상적으로 이것을 부모의 글로벌 포지션과 글로벌 포지션을 계산한 로컬 포지션으로 사용하고싶다.
		/// 불행하게도 이 수식은 상속 타입이 부모이외 일때 유지되지않는다.
		/// 부모의 rotation 과 scaling을 계산하는것은 RrSs 와 Rrs 케이스에서 힘든일이다. 
        lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
    }
    return lGlobalPosition;
}
// Get the matrix of the given pose
/// 주어진 포즈의 행렬을 얻는다.
FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
{
    FbxAMatrix lPoseMatrix;
    FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

    memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

    return lPoseMatrix;
}

// Get the geometry offset to a node. It is never inherited by the children.
/// 노드의 Geometry offset을 얻는다. 그것은 절대 자식에의해 상속된것이 아니다. 
FbxAMatrix GetGeometry(FbxNode* pNode)
{
    const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(lT, lR, lS);
}

