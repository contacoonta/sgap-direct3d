#include "DXUT.h"
#include "SDKmisc.h"
#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
}

ResourceManager::ResourceManager( const ResourceManager& )
{
}

ResourceManager::~ResourceManager()
{
	RemoveTextureAll();
}

/*
*/
ID3D11ShaderResourceView* ResourceManager::GetTexture(wstring key)
{
	// texture map 에 데이터가 없다면, null 리턴
	if (m_textures.size() == 0)
		return nullptr;

	// 해당 키의 텍스쳐가 있다면
	auto x = m_textures.find(key);

	// 못찾았다, 없다 , 
	if (x == m_textures.end())
		return nullptr;
	
	// 찾았다, 있다.
	return x->second;
}

ID3D11ShaderResourceView* ResourceManager::AddTextureFromFile(wstring key/*, ID3D11ShaderResourceView* srv*/)
{
	// 기존 texture 가 있다면, texture 리턴
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv)
		return psrv;

	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), key.c_str());

	//텍스쳐 파일 불러오기 ( .DDS )	
	HRESULT hr = DXUTCreateShaderResourceViewFromFile(DXUTGetD3D11Device(), strpathW, &psrv);
	if (FAILED(hr))
		return nullptr;

	// 새로운 texture 라면 , 
	//m_textures.insert(make_pair(key, srv));
	m_textures[key] = psrv;

	return psrv;
}

void ResourceManager::RemoveTexture(wstring key)
{
	// 기존 texture 가 없다면, 리턴
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv == nullptr)
		return;

	// texture 가 있다면, 지우기.
	SAFE_RELEASE(psrv);

	// map 데이터도 지우기.
	m_textures.erase(key);
}

void ResourceManager::RemoveTextureAll()
{
	// texture 부터 지우기
	for (auto x : m_textures)
	{
		SAFE_RELEASE(x.second);
	}

	// 맵의 모든 데이터 지우기
	m_textures.clear();
}