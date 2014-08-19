#pragma once

#include <xnamath.h>


typedef struct ConstantBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	XMFLOAT4 litDir;		// ������ ����
	XMFLOAT4 litCol;		// ������ ��

}CONSTANTBUFFER;


class CompileShader
{
public:
	CompileShader();
	~CompileShader();

	static HRESULT	Create(CompileShader** ppshader, WCHAR* wfilename, D3D11_INPUT_ELEMENT_DESC pLayout[], UINT numElements);
	static void		Delete(CompileShader** ppshader);

	void			RenderPrepare(const void* psrcData);

private:
	HRESULT			Initialize(WCHAR* wfilename, D3D11_INPUT_ELEMENT_DESC pLayout[], UINT numElements);
	void			Release();

	HRESULT			ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);

private:
	ID3D11InputLayout*      m_vertexlayout = nullptr;
	ID3D11VertexShader*     m_vertexshader = nullptr;
	ID3D11PixelShader*      m_pixelshader = nullptr;

	ID3D11Buffer*           m_constantbuffer = nullptr;
};
