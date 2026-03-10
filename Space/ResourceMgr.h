#pragma once

class Texture;

// 텍스처 리소스를 관리하는 싱글톤 매니저
class ResourceMgr : public Singleton<ResourceMgr>
{
private:
	// 파일명을 key로, 생성된 Texture를 보관
	std::map<std::wstring, Texture*> m_TextureMap;


public:
	ResourceMgr();
	~ResourceMgr();

	// 내부 자원 해제
	void Release();
	// 파일에서 텍스처를 생성하거나,
	// 이미 있으면 기존 텍스처를 반환
	Texture* CreateTextureFromFile(const std::wstring& fileName, D3DCOLOR ColorKey);
};