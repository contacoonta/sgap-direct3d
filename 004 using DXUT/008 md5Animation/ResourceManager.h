/*
	1. ���ӻ��� ���ҽ����� �Ѱ����� ����
	2. �ΰ��̻��� ����� Ȱ��
	3. �Ŵ����� Singleton ������ ���� Ȱ��.
*/

/*
	MAP < Ű , ������ >
	1. �������� , �������� ����
	2. ã�� ���� �ϱ����� ���� ( Ű )
*/

#pragma once

#include "base.h"
#include <map>
#include <string>

using std::map;
using std::wstring;



class ResourceManager
{
SINGLETON(ResourceManager);

//textures
public:
	ID3D11ShaderResourceView*	GetTexture(wstring key);
	ID3D11ShaderResourceView*	AddTextureFromFile(wstring key);
	void						RemoveTexture(wstring key);
	void						RemoveTextureAll();

//mesh - static

	
private:
	map<wstring, ID3D11ShaderResourceView*> m_textures;
};

#define RESMGR ResourceManager::instance()

