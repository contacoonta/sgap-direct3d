
cbuffer CONSTANTFORMAT : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

typedef struct vertexFormat
{
	float4 pos		: POSITION;

}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
}PIXELFORM;



PIXELFORM VS(VERTEXFORM input)
{
	PIXELFORM output = (PIXELFORM)0;
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	return output;
}


float4 PS(PIXELFORM input) : SV_TARGET
{
    return float4( 0.0f, 0.8f, 0.4f, 1.0f );
}
