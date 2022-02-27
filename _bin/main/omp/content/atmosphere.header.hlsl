float3       lightPosition;
float3        viewPosition;
float3              viewUp;
float4x4    viewProjMatrix;
float4x4 invViewProjMatrix;

struct InputVertex
{
	float3     vertex : LOCATION0;
	float2   texcoord : LOCATION1;
	float3   position : LOCATION2;
	float       scale : LOCATION3;
	float4   rotation : LOCATION4;
	float4      color : LOCATION5;
	float4 parameters : LOCATION6;
};

struct OutputVertex
{
	float4     ppsPosition : SV_POSITION;
	float4       clipSpace : POSITION0;
	float2           radii : POSITION1;
	float3          center : POSITION2;
	float3  lightDirection : POSITION3;
	float4         diffuse : COLOR0;
	float2        texcoord : TEXCOORD0;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
	float4  glow : SV_TARGET1;
};

float phase(float cos_theta)
{
 	return 0.5 * ((1 + cos_theta * cos_theta) * pow(0.75 / (1.25 - 0.5 * cos_theta), 1.5));
}