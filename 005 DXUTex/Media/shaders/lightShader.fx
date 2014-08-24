
Texture2D		texDiff : register(t0);
SamplerState	texSamp : register(s0);

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
	float2 tex		: TEXCOORD;
	float3 nor		: NORMAL;
	float3 tan		: TANGENT;
}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 nor		: NORMAL;
	float3 tan		: TANGENT;
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

	output.tex		= input.tex;
	
	output.nor		= mul( input.nor, world );
	output.tan		= mul( input.tan, world );

	return output;
}

/*
	Pixel Shader ( LIGHTING )
*/
float4 PS(PIXELFORM input) : SV_TARGET
{
	float4 outColor = 0;

	float fdot = dot( (float3)litDir, input.nor );
	outColor += texDiff.Sample(texSamp, input.tex) * saturate(fdot * litCol);
	outColor.a = 1;

	return outColor ;
}
