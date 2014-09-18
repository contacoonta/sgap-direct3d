/*
	1. 게임상의 리소스들을 한곳에서 관리
	2. 두개이상은 복사로 활용
	3. 매니져는 Singleton 디자인 패턴 활용.
*/

/*
	MAP < 키 , 데이터 >
	1. 오름차순 , 내림차순 정렬
	2. 찾기 쉽게 하기위한 정렬 ( 키 )
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

