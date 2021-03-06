/*
	TEXTURE
*/
Texture2D texDiff : register(t0);
SamplerState linSamp : register(s0);

/*
	CONSTANT BUFFER
*/
cbuffer CBNeverChanges : register (b0)
{
	matrix view;	
}

cbuffer CBChangeOnResize : register (b1)
{
	matrix projection;
}

cbuffer CBChangesEveryFrame : register (b2)
{
	matrix world;

	float4 litDir[2];
	float4 litCol[2];
	float4 colOutput;
}


/*
	VERTEX & PIXEL FORMAT
*/
typedef struct vertexFormat
{
	float4 pos		: POSITION;
	float3 nor		: NORMAL;
	float2 tex		: TEXCOORD0;
}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
	float3 nor		: TEXCOORD0;
	float2 tex		: TEXCOORD1;
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
float4 PSLight(PIXELFORM input) : SV_TARGET
{
	float4 outColor = 0;

	for (int i = 0; i < 2; i++)
	{
		float fdot = dot( (float3)litDir[i], input.nor );
		outColor += saturate(fdot * litCol[i]);
	}
	outColor.a = 1;

	return texDiff.Sample(linSamp, input.tex) * outColor;
}

/*
	
*/
float4 PSSolid( PIXELFORM input) : SV_TARGET
{
	return colOutput;
}
