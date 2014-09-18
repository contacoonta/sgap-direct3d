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
	// texture map �� �����Ͱ� ���ٸ�, null ����
	if (m_textures.size() == 0)
		return nullptr;

	// �ش� Ű�� �ؽ��İ� �ִٸ�
	auto x = m_textures.find(key);

	// ��ã�Ҵ�, ���� , 
	if (x == m_textures.end())
		return nullptr;
	
	// ã�Ҵ�, �ִ�.
	return x->second;
}

ID3D11ShaderResourceView* ResourceManager::AddTextureFromFile(wstring key/*, ID3D11ShaderResourceView* srv*/)
{
	// ���� texture �� �ִٸ�, texture ����
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv)
		return psrv;

	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), key.c_str());

	//�ؽ��� ���� �ҷ����� ( .DDS )	
	HRESULT hr = DXUTCreateShaderResourceViewFromFile(DXUTGetD3D11Device(), strpathW, &psrv);
	if (FAILED(hr))
		return nullptr;

	// ���ο� texture ��� , 
	//m_textures.insert(make_pair(key, srv));
	m_textures[key] = psrv;

	return psrv;
}

void ResourceManager::RemoveTexture(wstring key)
{
	// ���� texture �� ���ٸ�, ����
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv == nullptr)
		return;

	// texture �� �ִٸ�, �����.
	SAFE_RELEASE(psrv);

	// map �����͵� �����.
	m_textures.erase(key);
}

void ResourceManager::RemoveTextureAll()
{
	// texture ���� �����
	for (auto x : m_textures)
	{
		SAFE_RELEASE(x.second);
	}

	// ���� ��� ������ �����
	m_textures.clear();
}