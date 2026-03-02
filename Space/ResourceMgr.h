#pragma once

class Texture;
class ResourceMgr : public Singleton<ResourceMgr>
{
private:
	std::map<std::wstring, Texture*> m_TextureMap;

	void Release();
public:
	ResourceMgr();
	~ResourceMgr();

	Texture* CreateTextureFromFile(std::wstring fileName, D3DCOLOR ColorKey);

};

