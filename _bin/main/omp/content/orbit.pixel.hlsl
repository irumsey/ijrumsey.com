#include "utility.header.hlsl"
#include "orbit.header.hlsl"
 
Texture2D orbitLookup;

SamplerState theSampler;

OutputPixel main(InputPixel input)
{
	OutputPixel output = (OutputPixel)0;

	output.color = input.color;
	output.id = input.id;

	return output; 
}
