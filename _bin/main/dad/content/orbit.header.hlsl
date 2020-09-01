float4x4 viewProjMatrix;

struct InputVertex
{
	float2   position : LOCATION0;
	float2    adapted : LOCATION1;
	float2     select : LOCATION2;

	float3      focus : LOCATION3;
	float       scale : LOCATION4;
	float4   rotation : LOCATION5;
	float4      color : LOCATION6;
	float4 parameters : LOCATION7;

/*
	float4 parameters : LOCATION3;	parameters -> parameters
	float3     center : LOCATION4;	center -> focus
	float4   rotation : LOCATION5;  rotation -> rotation
	float4  lineColor : LOCATION6;  lineColor -> color
	float1  lineWidth : LOCATION7;  lineWidth -> scale
*/
};

struct OutputVertex
{
	float4  parameters :   POSITION0;
	float2    position :   POSITION1;
	float        theta :   POSITION2;
	float    lineWidth :   POSITION3;
	float4   lineColor :      COLOR0;
	float4 ppsPosition : SV_POSITION;
};

typedef OutputVertex InputPixel;

struct OutputPixel
{
	float4 color : SV_TARGET0;
};

float2 computeConicPoint(float hu, float e, float theta)
{
	float c = cos(theta);
	float s = sin(theta);
	float r = hu / (1 + e * c);

	return r * float2(c, s);
}
