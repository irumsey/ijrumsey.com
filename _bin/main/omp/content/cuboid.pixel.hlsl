#include "cuboid.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	// diffuse should come from a texture...
	float3 diffuse = float3(1, 1, 1);
	float3 color = (diffuse.rgb - input.ambient.rgb) * input.shade + input.ambient.rgb;

	output.color = float4(color, 1);
	output. glow = float4(0, 0, 0, 1);
	output.   id = input.id;

	return output;
}
