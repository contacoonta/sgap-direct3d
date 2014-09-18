#pragma once


typedef struct Vertex
{
	XMFLOAT3 pos;		// ��ġ ����
	XMFLOAT3 nor;		// ���� ����
	XMFLOAT3 tex;		// �ؽ��� ��ǥ ����

}VERTEX, *LPVERTEX;


/*
	Obj Loader -> Mesh Static 
*/
class MeshStatic : public Mesh
{
public:
	friend class LoaderObj;

public:
	MeshStatic();
	~MeshStatic();

	Mesh*		Clone();

	HRESULT		Initialize();
	void		Render(/* CompileShader* pshader */);
	void		Release();

private:
	// �޽��� ���ؽ� ����
	ID3D11Buffer*	m_vertexBuff = nullptr;
	// �޽��� �ε��� ����
	ID3D11Buffer*	m_indexBuff = nullptr;
	// �ε��� ��
	UINT			m_indexCnt = 0;

	// for texture
	ID3D11ShaderResourceView*	m_textureClone = nullptr;	// �ؽ���

};