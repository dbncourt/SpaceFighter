////////////////////////////////////////////////////////////////////////////////
// Filename: TexturePixelShader.hlsl
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 color;

	color = shaderTexture.Sample(SampleType, input.tex);

	if (color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.a = 1.0f;
	}

	return color;
}