#include "utility.header.hlsl"
#include "orbit.header.hlsl"
 
OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	// do not move this to the vertex shader, you should know why.
	float blend = 1 - input.blend * input.blend;

	output.color = float4(input.color.rgb, blend);
	output.glow = float4(0, 0, 0, 0);
	output.id = input.id;

	return output; 
}
