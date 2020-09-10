#include "sphere.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float3 position = input.position;
	float     scale = input.scale;
	float2   vertex = scale.xx * input.vertex;

	float3 e1 = normalize(viewPosition - position);
	float3 e0 = cross(e1, viewUp);
	float3 e2 = cross(e0, e1);

	position = position + e0 * vertex.xxx + e2 * vertex.yyy;

	///	rotation to put light direction into texture space.
	///	swizzling y and z since z is world up and y is texuture space up
	float3x3 R = float3x3(e0, e2, e1);

	output.   ppsPosition = mul(viewProjMatrix, float4(position, 1));
	output.lightDirection = mul(R, normalize(lightPosition - position));
	output.       diffuse = input.color;
	output.      texcoord = 0.5 * input.vertex + 0.5;
	
	return output; 
}
