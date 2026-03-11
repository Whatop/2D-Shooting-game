#include "stdafx.h"
#include "Sprite.h"
#include "Texture.h"
#include "ResourceMgr.h"

Sprite::Sprite()
	: m_pSp(Renderer::GetInst()->GetSprite()) // Renderer가 가진 공용 Sprite 객체 사용
	, m_Texture(nullptr)
{
	// 공용 Sprite를 참조하므로 참조 카운트 증가
	if (m_pSp)
		m_pSp->AddRef();

	SetRect(&m_Rect, 0, 0, 0, 0);

	// 기본 색상은 완전 불투명 흰색
	A = 255;
	R = 255;
	G = 255;
	B = 255;
}

Sprite::~Sprite()
{
	// 생성자에서 AddRef 했으므로 소멸 시 Release 필요
	if (m_pSp)
	{
		m_pSp->Release();
		m_pSp = nullptr;
	}
}

// 텍스처 파일을 로드해서 Sprite 초기화
bool Sprite::Init(std::wstring fileName, D3DCOLOR ColorKey)
{
	m_Texture = ResourceMgr::GetInst()->CreateTextureFromFile(fileName, ColorKey);

	if (m_Texture)
	{
		// 텍스처 크기를 가져와서 전체 영역을 그리도록 rect 설정
		m_Size = m_Texture->GetSize();
		SetRect(&m_Rect, 0, 0, static_cast<int>(m_Size.x), static_cast<int>(m_Size.y));
		return true;
	}
	else
		return false;
}

// Sprite 생성 헬퍼 함수
Sprite* Sprite::Create(std::wstring fileName, D3DCOLOR ColorKey)
{
	auto sprite = new (std::nothrow) Sprite();

	if (sprite && sprite->Init(fileName, ColorKey))
	{
		return sprite;
	}
	else
	{
		SafeDelete(sprite);
		return nullptr;
	}
}

void Sprite::Render()
{
	// 렌더링에 필요한 객체가 없으면 그리지 않음
	if (!m_pSp || !m_Texture)
		return;

	// 부모 오브젝트가 있으면 부모 기준 충돌 범위를 갱신
	if (m_Parent)
	{
		// Player는 스케일을 무시하고 원본 크기 기준 충돌 박스 설정
		if (m_Parent->m_Tag == "Player") {
			SetRect(&m_Parent->m_Collision,
				static_cast<int>(m_Parent->m_Position.x - (m_Size.x * 1) / 2),
				static_cast<int>(m_Parent->m_Position.y - (m_Size.y * 1) / 2),
				static_cast<int>(m_Parent->m_Position.x + (m_Size.x * 1) / 2),
				static_cast<int>(m_Parent->m_Position.y + (m_Size.y * 1) / 2));
		}
		else {
			// 일반 오브젝트는 부모 스케일까지 반영한 충돌 박스 설정
			SetRect(&m_Parent->m_Collision,
				static_cast<int>(m_Parent->m_Position.x - (m_Size.x * m_Parent->m_Scale.x) / 2),
				static_cast<int>(m_Parent->m_Position.y - (m_Size.y * m_Parent->m_Scale.y) / 2),
				static_cast<int>(m_Parent->m_Position.x + (m_Size.x * m_Parent->m_Scale.x) / 2),
				static_cast<int>(m_Parent->m_Position.y + (m_Size.y * m_Parent->m_Scale.y) / 2));
		}

		// 부모에게 자신의 크기 정보 전달
		m_Parent->m_Size = m_Size;
	}
	else
	{
		// 부모가 없으면 자기 자신의 충돌 범위 설정
		SetRect(&m_Collision,
			static_cast<int>(m_Position.x - (m_Size.x * m_Scale.x) / 2),
			static_cast<int>(m_Position.y - (m_Size.y * m_Scale.y) / 2),
			static_cast<int>(m_Position.x + (m_Size.x * m_Scale.x) / 2),
			static_cast<int>(m_Position.y + (m_Size.y * m_Scale.y) / 2));
	}

	// 카메라 행렬 갱신
	Camera::GetInst()->Render();

	// 알파 블렌딩 모드로 Sprite 렌더 시작
	m_pSp->Begin(D3DXSPRITE_ALPHABLEND);

	// 기본은 카메라 월드행렬 * 내 변환행렬
	Matrix transForm = Camera::GetInst()->GetWorld() * GetMatrix();

	// UI는 카메라 영향을 받지 않고 화면 고정
	if (m_Tag == "UI")
		transForm = GetMatrix();
	else
		transForm = Camera::GetInst()->GetWorld() * GetMatrix();

	m_pSp->SetTransform(&transForm);

	// 중심점을 이미지 중앙으로 설정
	D3DXVECTOR3 center = D3DXVECTOR3(m_Size.x / 2, m_Size.y / 2, 0);

	// Visible이 false면 완전 투명으로 그림
	if (m_Visible == false)
		m_pSp->Draw(m_Texture->GetTexture(), &m_Rect, &center, 0, D3DCOLOR_ARGB(0, R, G, B));
	else
		m_pSp->Draw(m_Texture->GetTexture(), &m_Rect, &center, 0, D3DCOLOR_ARGB(A, R, G, B));

	m_pSp->End();
}