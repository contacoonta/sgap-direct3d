
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
}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
	float3 nor		: TEXCOORD0;
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

	return output;
}

/*
	Pixel Shader ( LIGHTING )
*/
float4 PS(PIXELFORM input) : SV_TARGET
{
	float4 outColor = 0;

	float fdot = dot( (float3)litDir, input.nor );
	outColor += saturate(fdot * litCol);
	outColor.a = 1;
	//outColor = litCol;

	return outColor;
}
