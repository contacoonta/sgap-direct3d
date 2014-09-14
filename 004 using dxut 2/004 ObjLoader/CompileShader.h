#pragma once


using namespace DirectX;

typedef struct ConstantBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	XMFLOAT4 litDir;		// ������ ����
	XMFLOAT4 litCol;		// ������ ��

}CONSTANTBUFFER;


class CompileShader
{
public:
	CompileShader();
	~CompileShader();

	static HRESULT	Create(CompileShader** ppshader, WCHAR* wfilename, WCHAR* wtexturefilename = NULL );
	static void		Delete(CompileShader** ppshader);

	void			RenderPrepare(const void* psrcData);

private:
	HRESULT			Initialize(WCHAR* wfilename);
	void			Release();

	HRESULT			ComplieShaderFromFile(WCHAR* wFilename, LPCSTR strEntry, LPCSTR strShaderMdl, ID3DBlob** ppblob);
public:
	HRESULT			CreateTextureFromFile(WCHAR* wfilename);

private:
	ID3D11InputLayout*      m_vertexlayout = nullptr;
	ID3D11VertexShader*     m_vertexshader = nullptr;
	ID3D11PixelShader*      m_pixelshader = nullptr;

	ID3D11Buffer*           m_constantbuffer = nullptr;

	// for texture
	ID3D11ShaderResourceView*	m_textureRView = nullptr;	// �ؽ���
	ID3D11SamplerState*         m_samplerLinear = nullptr;	// �ؽ��� ���ø�
};

