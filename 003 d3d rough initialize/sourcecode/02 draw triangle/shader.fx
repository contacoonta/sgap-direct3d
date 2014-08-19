
/*
	πˆ≈ÿΩ∫ ºŒ¿Ã¥ı
*/
float4 VS( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}


/*
	«»ºø ºŒ¿Ã¥ı
*/
float4 PS( float4 pos : SV_POSITION ) : SV_TARGET
{
	return float4(1.0f, 0.2f, 0.0f, 1.0f);
}