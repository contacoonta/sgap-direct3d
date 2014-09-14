#pragma once


typedef struct Vertex
{
	XMFLOAT3 pos;		// 위치 정보
	XMFLOAT3 nor;		// 방향 정보
	XMFLOAT3 tex;		// 텍스쳐 좌표 정보

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
	// 메시의 버텍스 버퍼
	ID3D11Buffer*	m_vertexBuff = nullptr;
	// 메시의 인덱스 버퍼
	ID3D11Buffer*	m_indexBuff = nullptr;
	// 인덱스 수
	UINT			m_indexCnt = 0;

	// for texture
	ID3D11ShaderResourceView*	m_textureClone = nullptr;	// 텍스쳐

};