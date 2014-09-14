#include "DXUT.h"
#include "SDKmisc.h"
#include "ResourceManager.h"


/*
*/
ResourceManager::ResourceManager()
{
}

ResourceManager::ResourceManager(const ResourceManager& resmgr)
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
	// texture map 에 데이터 없다면, null 리턴
	if (m_textures.size() == 0 )
		return nullptr;

	// 해당 키의 텍스쳐가 있는지 조회
	auto x = m_textures.find(key);

	// 없으면 , null 리턴
	if (x == m_textures.end())
		return nullptr;

	// 있으면 , texture 리턴
	return x->second;
}

ID3D11ShaderResourceView* ResourceManager::AddTextureFromFile(wstring key)
{
	// 기존 texture 가 있다면, texture 리턴
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv)
		return psrv;

	// 없으면 텍스쳐를 새로 만들어서 리턴
	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), key.c_str());

	//텍스쳐 파일 불러오기 ( .DDS )	
	HRESULT hr = DXUTCreateShaderResourceViewFromFile(DXUTGetD3D11Device(), strpathW, &psrv);
	if (FAILED(hr))
	{
		//에러 메시지 출력
		std::wstring message = L"Could not open TEXTURE :  ";
		message += strpathW;
		MessageBox(NULL, message.c_str(), L"Error", MB_OK);
		return nullptr;
	}

	m_textures[key] = psrv;

	return psrv;
}

void ResourceManager::RemoveTexture(wstring key)
{
	// 기존 texture 가 없다면, 아무것도 안하고 리턴
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv == nullptr)
		return;

	// texture 가 있다면, 지우기
	SAFE_RELEASE(psrv);

	// map 데이터도 지우기
	m_textures.erase(key);
}

void ResourceManager::RemoveTextureAll()
{
	// 모든 texture 지우기
	for (auto x : m_textures)
	{
		SAFE_RELEASE(x.second);
	}

	// 모든 데이터 지우기
	m_textures.clear();
}