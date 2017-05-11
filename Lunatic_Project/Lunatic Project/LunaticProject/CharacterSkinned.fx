#include "Light.fx"

//
// 상수 버퍼
///////////////////////////////////////////////////////////////////////////
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
}

cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);

}

cbuffer cbSkinned : register(b2)
{
	// 한 캐릭터 당 최대 뼈대 개수는 96
	matrix gBoneTransform[96];// : packoffset(c0);
};


//
// 쉐이더 변수
///////////////////////////////////////////////////////////////////////////
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

struct VS_SKINNED_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4 weights : WEIGHTS;
	float4 boneindices : BONEINDICES;
};

struct VS_SKINNED_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};

VS_SKINNED_OUTPUT VSSkinned(VS_SKINNED_INPUT input)
{
	VS_SKINNED_OUTPUT output = (VS_SKINNED_OUTPUT)0;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.weights.x;
	weights[1] = input.weights.y;
	weights[2] = input.weights.z;
	weights[3] = 1 - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		posL += weights[i] * mul(float4(input.position, 1.0f), gBoneTransform[input.boneindices[i]]).xyz;
		normalL += weights[i] * mul(input.normal, (float3x3)gBoneTransform[input.boneindices[i]]);
	}
	
	output.positionW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.normalW = mul(normalL, (float3x3)gmtxWorld);
	output.texCoord = input.texCoord;

	return output;
}

float4 PSSkinned(VS_SKINNED_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
		float4 cColor = gtxtTexture.Sample(gSamplerState, input.texCoord) * cIllumination;

		return(cColor);
}