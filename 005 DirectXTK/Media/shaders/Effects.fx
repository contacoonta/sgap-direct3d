cbuffer cbPerObject
{
	float4x4 WVP;
    float4x4 World;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};


VS_OUTPUT D2D_VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;
	
    output.Pos = mul(inPos, WVP);
	output.worldPos = mul(inPos, World);

	output.normal = mul(normal, World);

    output.TexCoord = inTexCoord;

    return output;
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
	
	return diffuse;
}