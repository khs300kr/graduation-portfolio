#include "/Client/Effect.fx"

#define PARTICLE_TYPE_EMITTER	0
#define PARTICLE_TYPE_FLARE		1

cbuffer cbParticleInfo : register(b2)// 파티클 정보 상수 버퍼.
{
	float3 gvParticleEmitPosition;
	float gfGameTime;
	float3 gvParticleEmitDirection;
	float gfTimeStep;
	float3 gvAcceleration;
};

Texture1D gtxtRandomTexture : register(t0);
Texture2D gtxtParticleTextureArray : register(t5);
SamplerState gParticleSamplerState : register(s0);

struct PARTICLE_INPUT
{
	float3 position : POSITION;
	float3 velocity : VELOCITY;
	float2 size : SIZE;
	uint type : TYPE;
	float age : AGE;
};

struct PARTICLE_OUTPUT
{
	float3 position : POSITION;
	float2 size : SIZE;
	float4 color : COLOR;
	uint type : TYPE;
};

struct GS_PARTICLE_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};


PARTICLE_INPUT VSParticleStreamOut(PARTICLE_INPUT input)
{
	return input;
}


PARTICLE_OUTPUT VSParticleDraw(PARTICLE_INPUT input)
{
	PARTICLE_OUTPUT output;

	float t = input.age;
	output.position = (0.5f * gvAcceleration * t * t) + (input.velocity * t) + input.position;

	float fOpacity = 1.0f - smoothstep(0.0f, 1.0f, t);

	output.color = float4(1.0f, 1.0f, 1.0f, fOpacity);
	output.size = input.size;
	output.type = input.type;

	return output;
}


[maxvertexcount(2)]
void GSParticleStreamOut(point PARTICLE_INPUT input[1], inout PointStream<PARTICLE_INPUT> pointStream)
{
	input[0].age += gfTimeStep;
	if (input[0].type == PARTICLE_TYPE_EMITTER)
	{
		if (input[0].age > 0.005f)
		{
			float3 vRandom = gtxtRandomTexture.SampleLevel(gParticleSamplerState, gfGameTime, 0).xyz;
			vRandom = normalize(vRandom);
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;
			PARTICLE_INPUT particle = (PARTICLE_INPUT)0;
			particle.position = gvParticleEmitPosition.xyz;
			particle.velocity = 4.0f * vRandom;
			particle.size = float2(1.0f, 1.0f);
			particle.age = 0.0f;
			particle.type = PARTICLE_TYPE_FLARE;
			pointStream.Append(particle);
			input[0].age = 0.0f;
			input[0].type = PARTICLE_TYPE_EMITTER;	// 필요한가...
		}
		pointStream.Append(input[0]);
	}
	else
		if (input[0].age <= 1.0f) pointStream.Append(input[0]);
}

[maxvertexcount(4)]
void GSParticleDraw(point PARTICLE_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_OUT> triStream)
{
	if (input[0].type == PARTICLE_TYPE_EMITTER) return;

	float3 vLook = normalize(gvCameraPosition.xyz - input[0].position);
		float3 vRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), vLook));
		float3 vUp = cross(vLook, vRight);

		float fHalfWidth = 0.5f * input[0].size.x, fHalfHeight = 0.5f * input[0].size.y;
	float4 vQuad[4];
	vQuad[0] = float4(input[0].position + fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
	vQuad[1] = float4(input[0].position + fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
	vQuad[2] = float4(input[0].position - fHalfWidth * vRight - fHalfHeight * vUp, 1.0f);
	vQuad[3] = float4(input[0].position - fHalfWidth * vRight + fHalfHeight * vUp, 1.0f);
	matrix mtxViewProjection = mul(gmtxView, gmtxProjection);
	GS_PARTICLE_OUT output = (GS_PARTICLE_OUT)0;

	float2 TexCoord[4] =
	{
		float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f)
	};

	for (int i = 0; i < 4; i++)
	{
		output.position = mul(vQuad[i], mtxViewProjection);
		output.texCoord = TexCoord[i];
		output.color = input[0].color;
		triStream.Append(output);
	}
}


float4 PSParticleDraw(GS_PARTICLE_OUT input) : SV_Target
{
	float4 cColor = gtxtParticleTextureArray.Sample(gParticleSamplerState, input.texCoord);
	return(cColor * input.color);
}