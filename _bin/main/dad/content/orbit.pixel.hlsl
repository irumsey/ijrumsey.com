#include "orbit.header.hlsl"

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	float u = clamp(2 * length(input.position - input.vertex) / lineWidth, 0, 1);
	output.color = float4(lineColor.rgb, 1 - u * u);

	return output;
}
