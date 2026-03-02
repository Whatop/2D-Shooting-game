#include "stdafx.h"
#include "ResourceMgr.h"
#include"Texture.h"

ResourceMgr::ResourceMgr()
{
}


ResourceMgr::~ResourceMgr() { Release(); }

void ResourceMgr::Release() {
    for (auto it = m_TextureMap.begin(); it != m_TextureMap.end(); ++it) {
        SafeDelete(it->second);
    }
    m_TextureMap.clear();
}

Texture* ResourceMgr::CreateTextureFromFile(std::wstring fileName, D3DCOLOR ColorKey)
{
	if (!(m_TextureMap.count(fileName)))
	{
		auto texture = new (std::nothrow) Texture();
		if (texture && texture->Init(fileName, ColorKey))
		{
			m_TextureMap[fileName] = texture;
		}
		else
		{
			SafeDelete(texture);
			return 0;
		}
	}

	return m_TextureMap[fileName];
}
