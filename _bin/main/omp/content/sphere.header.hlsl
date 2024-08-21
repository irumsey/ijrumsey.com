float3    lightPosition;
float3     viewPosition;
float4x4 viewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float3    tangent : LOCATION1;
	float2   texcoord : LOCATION2;
	uint1          id : LOCATION3;
	float4   position : LOCATION4;	// position.w is scale
	float4   rotation : LOCATION5;
	float4    diffuse : LOCATION6;
	float4    ambient : LOCATION7;
	float4 parameters : LOCATION8;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float2        texcoord : TEXCOORD0;
	float3  lightDirection : POSITION0;
	float3   viewDirection : POSITION1;
	uint1               id : POSITION2;
	float4         diffuse : COLOR0;
	float4         ambient : COLOR1;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
	uint1     id : SV_TARGET2;
};
