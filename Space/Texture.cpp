#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: m_pDev(Renderer::GetInst()->GetDevice())
	, m_pTexture(nullptr)
	, m_Size(0.f, 0.f)
{
	if (m_pDev)
		m_pDev->AddRef();
}

Texture::~Texture()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	if (m_pDev)
	{
		m_pDev->Release();
		m_pDev = nullptr;
	}
}

bool Texture::Init(std::wstring fileName, D3DCOLOR ColorKey)
{
	HRESULT hr;
	hr = D3DXCreateTextureFromFileEx(
		m_pDev,
		fileName.c_str(),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		ColorKey,
		0,
		0,
		&m_pTexture);

	if FAILED(hr)
	{
		MessageBox(0, L"텍스쳐 생성 실패", 0, MB_OK);
		return false;
	}

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);

	m_Size.x = static_cast<float>(desc.Width);
	m_Size.y = static_cast<float>(desc.Height);

	return true;
}