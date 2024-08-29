#include "instance.header.hlsl"

float3   lightDirection;	// unit direction from view position to light position
float4x4 viewProjMatrix;

struct Vertex
{
	float3   normal : LOCATION0;
	float3  tangent : LOCATION1;
	float2 texcoord : LOCATION2;
};

struct InputVS
{
	Vertex      vertex;
	Instance  instance;
};

struct OutputVS
{
	float4     ppsPosition : SV_POSITION;
	float2        texcoord : TEXCOORD0;
	float3   viewDirection : POSITION0;
	float3  lightDirection : POSITION1;
	uint1               id : POSITION2;
	float        specCoeff : COLOR0;
	float4         ambient : COLOR1;
};

typedef OutputVS InputPS;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};
