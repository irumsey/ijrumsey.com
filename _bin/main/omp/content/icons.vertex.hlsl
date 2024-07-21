#include "icons.header.hlsl"

OutputVertex main(InputVertex input)
{
	OutputVertex output = (OutputVertex)0;

	float4 ppsPosition = mul(viewProjMatrix, float4(input.position, 1));
	ppsPosition = ppsPosition / ppsPosition.w;
	ppsPosition.z = min(ppsPosition.z, 0.999);

	ppsPosition.xy = ppsPosition.xy + texelSize * input.vertex * input.scale;

	float2 texcoord = 0.5 * input.vertex + float2(0.5, 0.5);

	output.ppsPosition = ppsPosition;
	output.texcoord = input.texcoord.xy + texcoord * input.texcoord.zw;
	output.color = input.color;
	output.id = input.id;

	return output;
}
