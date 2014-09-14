#pragma once

#include "basedef.h"
#include <map>
#include <string>

using std::map;
using std::wstring;


class ResourceManager
{
SINGLETON(ResourceManager);

//TEXTURES
public:
	ID3D11ShaderResourceView*	GetTexture(wstring key);
	ID3D11ShaderResourceView*	AddTextureFromFile(wstring key/*, ID3D11ShaderResourceView* srv*/);
	void						RemoveTexture(wstring key);
	void						RemoveTextureAll();

private:
	map<wstring, ID3D11ShaderResourceView*> m_textures;
	
};

#define RESMGR ResourceManager::instance()

