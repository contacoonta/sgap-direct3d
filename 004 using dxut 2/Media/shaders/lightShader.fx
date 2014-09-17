Texture2D tex2d : register (t0);
SamplerState samp : register (s0);


cbuffer CONSTANTBUFFER : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;

	float4 litDir;
	float4 litCol;
}

typedef struct vertexFormat
{
	float4 pos		: POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD0;
}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD0;
}PIXELFORM;


/*
	Vertex Shader
*/
PIXELFORM VS(VERTEXFORM input)
{
	PIXELFORM output = (PIXELFORM)0;
	output.pos		= mul( input.pos, world );
	output.pos		= mul( output.pos, view );
	output.pos		= mul( output.pos, projection );
	
	output.nor		= mul( input.nor, world );
	output.tex		= input.tex;

	return output;
}

/*
	Pixel Shader ( LIGHTING )
*/
float4 PS(PIXELFORM input) : SV_TARGET
{
	float4 outColor = float4(0.15f, 0.16f, 0.14f, 1.0f);
	
	float fdot = dot( (float3)litDir, input.nor );
	outColor += saturate(fdot * litCol);

	outColor.a = 1;

	return tex2d.Sample(samp, input.tex) * outColor;
}
