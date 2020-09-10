#include "hemisphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position.xyz;
	float     scale = input.scale;
	float3   vertex = scale.xxx * input.vertex;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = cross(e1, viewUp);
	float3 e2 = cross(e0, e1);

	position = position + e0 * vertex.xxx + e2 * vertex.yyy + e1 * vertex.zzz;

	output.ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.    diffuse = input.color;

	return output; 
}
