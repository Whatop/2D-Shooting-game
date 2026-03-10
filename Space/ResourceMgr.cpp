#include "stdafx.h"
#include "ResourceMgr.h"
#include "Texture.h"

ResourceMgr::ResourceMgr()
{
}

ResourceMgr::~ResourceMgr()
{
	Release(); // 매니저 소멸 시 가지고 있던 텍스처 전부 해제
}

void ResourceMgr::Release()
{
	// map에 저장된 모든 Texture 삭제
	for (auto& pair : m_TextureMap)
	{
		SafeDelete(pair.second);
	}
	m_TextureMap.clear();
}

Texture* ResourceMgr::CreateTextureFromFile(const std::wstring& fileName, D3DCOLOR ColorKey)
{
	auto it = m_TextureMap.find(fileName);
	if (it != m_TextureMap.end())
		return it->second;

	Texture* texture = new (std::nothrow) Texture();
	if (texture == nullptr)
		return nullptr;

	if (!texture->Init(fileName, ColorKey))
	{
		SafeDelete(texture);
		return nullptr;
	}

	m_TextureMap[fileName] = texture;
	return texture;
}