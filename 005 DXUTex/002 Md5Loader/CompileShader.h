#pragma once

#include <DirectXMath.h>

using namespace DirectX;


typedef struct ConstantBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	XMFLOAT4 litDir;		// 조명의 방향
	XMFLOAT4 litCol;		// 조명의 색

}CONSTANTBUFFER;


class CompileShader
{
public:
	friend class MeshObj;
	friend class MeshMd5;

public:
	CompileShader();
	~CompileShader();

	static HRESULT	Create(CompileShader** ppshader, WCHAR* wfilename);
	static void		Delete(CompileShader** ppshader);

	void			RenderPrepare(const void* psrcData);

private:
	HRESULT			Initialize(WCHAR* wfilename);
	void			Release();

	HRESULT			ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);

private:
	ID3D11InputLayout*      m_vertexlayout = nullptr;
	ID3D11VertexShader*     m_vertexshader = nullptr;
	ID3D11PixelShader*      m_pixelshader = nullptr;

	ID3D11Buffer*           m_constantbuffer = nullptr;

	// for texture
	ID3D11SamplerState*		m_samplerLinear = nullptr;
};

