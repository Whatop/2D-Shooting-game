#include "stdafx.h"
#include "CollisionMgr.h"


CollisionMgr::CollisionMgr()
{
}


CollisionMgr::~CollisionMgr()
{
}

bool CollisionMgr::MouseWithBoxSize(Object* obj)
{
	// 오브젝트가 없으면 충돌하지 않음
	if (!obj) return false;

	// 현재 마우스 좌표를 정수형 POINT로 변환
	POINT mouse;
	mouse.x = (LONG)INPUT->GetMousePos().x;
	mouse.y = (LONG)INPUT->GetMousePos().y;

	// 마우스가 오브젝트의 충돌 영역(Rect) 안에 있으면 true
	return PtInRect(&obj->m_Collision, mouse);
}