
cbuffer CONSTANTBUFFER : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;

	float4 outcolor;
}

typedef struct vertexInput
{
	float4 pos		: POSITION;
	float4 color	: COLOR0;
}VERTEXINPUT;

typedef struct vertexOutput
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR0;
}VERTEXOUTPUT;


/*
	Vertex Shader
*/
VERTEXOUTPUT VS(VERTEXINPUT input)
{
	VERTEXOUTPUT output = (VERTEXOUTPUT)0;
	output.pos		= mul( input.pos, world );
	output.pos		= mul( output.pos, view );
	output.pos		= mul( output.pos, projection );
	output.color	= outcolor;

	return output;
}

/*
	Pixel Shader
*/
float4 PS(VERTEXOUTPUT input) : SV_TARGET
{
	return input.color;
}
