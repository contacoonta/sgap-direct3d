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
	// texture map �� ������ ���ٸ�, null ����
	if (m_textures.size() == 0 )
		return nullptr;

	// �ش� Ű�� �ؽ��İ� �ִ��� ��ȸ
	auto x = m_textures.find(key);

	// ������ , null ����
	if (x == m_textures.end())
		return nullptr;

	// ������ , texture ����
	return x->second;
}

ID3D11ShaderResourceView* ResourceManager::AddTextureFromFile(wstring key)
{
	// ���� texture �� �ִٸ�, texture ����
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv)
		return psrv;

	// ������ �ؽ��ĸ� ���� ���� ����
	WCHAR strpathW[256] = {};
	DXUTFindDXSDKMediaFileCch(strpathW, sizeof(strpathW) / sizeof(WCHAR), key.c_str());

	//�ؽ��� ���� �ҷ����� ( .DDS )	
	HRESULT hr = DXUTCreateShaderResourceViewFromFile(DXUTGetD3D11Device(), strpathW, &psrv);
	if (FAILED(hr))
	{
		//���� �޽��� ���
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
	// ���� texture �� ���ٸ�, �ƹ��͵� ���ϰ� ����
	ID3D11ShaderResourceView* psrv = GetTexture(key);
	if (psrv == nullptr)
		return;

	// texture �� �ִٸ�, �����
	SAFE_RELEASE(psrv);

	// map �����͵� �����
	m_textures.erase(key);
}

void ResourceManager::RemoveTextureAll()
{
	// ��� texture �����
	for (auto x : m_textures)
	{
		SAFE_RELEASE(x.second);
	}

	// ��� ������ �����
	m_textures.clear();
}