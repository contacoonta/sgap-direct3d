
cbuffer CONSTANTBUFFER : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

typedef struct vertexformat
{
	float4 pos		: SV_POSITION;
	float4 color	: COLOR0;
}VERTEXFORM;


/*
	Vertex Shader
*/
VERTEXFORM VS(float4 pos : POSITION, float4 color : COLOR)
{
	VERTEXFORM output = (VERTEXFORM)0;
	output.pos		= mul( pos, world );
	output.pos		= mul( output.pos, view );
	output.pos		= mul( output.pos, projection );
	output.color	= color;

	return output;
}

/*
	Pixel Shader
*/
float4 PS(VERTEXFORM input) : SV_TARGET
{
	return input.color;
}
