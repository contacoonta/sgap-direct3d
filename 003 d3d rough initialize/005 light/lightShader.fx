
// 외부의 값을 셰이더 내부로 전달 해줄 상수 변수
cbuffer CONSTANTBUFFER : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;

	float4 litDir;
	float4 litCol;
	float4 colOutput;
}

typedef struct vertexformat
{
	float4 pos	:	POSITION;
	float4 nor	:	NORMAL;

}VERTEXFORM;

typedef struct pixelFormat
{
	float4 pos		: SV_POSITION;
	float3 nor		: TEXCOORD0;
}PIXELFORM;



/*
*/
PIXELFORM VERTEXSHADER(VERTEXFORM input)
{
	PIXELFORM output = (PIXELFORM)0;

	// 기본 pos 값을 world 좌표로 변형
	output.pos = mul(input.pos, world);
	// world 좌표의 값을 카메라(눈) 좌표 값으로 변형
	output.pos = mul(output.pos, view);
	// view 좌표계에서 projection 자표계로 변형
	output.pos = mul(output.pos, projection);

	output.nor = mul(input.nor, world);

	return output;
}

/*
	Pixel Shader ( LIGHTING )
*/
float4 PSLight(PIXELFORM input) : SV_TARGET0
{
	float4 outColor = 0;

	float fdot = dot((float3)litDir, input.nor);
	outColor += saturate(fdot * litCol);
	
	outColor.a = 1;

	return outColor;
}


/*
	Pixel Shader ( SOLID )
*/
float4 PSSolid(PIXELFORM input) : SV_TARGET
{
	return colOutput;
}
