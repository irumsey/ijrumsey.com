float time;
float interpolant;

float3 viewPosition;
float3 lightPosition;

float3 viewRight;
float3 viewForward;
float3 viewUp;
float4x4 viewProjMatrix;

Texture2D normalsTexture;
SamplerState theSampler;

struct InputVertex
{
	float4   corner : LOCATION0;
	float4    color : LOCATION1;
	float4 position : LOCATION2;
};

struct OutputVertex
{
	float4    ppsPosition : SV_POSITION;
	float3 lightDirection : POSITION0;
	float4        diffuse : COLOR0;
	float2       texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};
