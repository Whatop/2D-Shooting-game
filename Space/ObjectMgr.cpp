#include "stdafx.h"
#include "ObjectMgr.h"



ObjectMgr::ObjectMgr()
{
}

ObjectMgr::~ObjectMgr()
{
}

void ObjectMgr::Release()
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end(); iter++)
	{
		(*iter)->SetDestroy(true);
	}
}

void ObjectMgr::DeleteCheak()
{
	for (auto iter = m_Objects.begin(); iter != m_Objects.end();)
	{
		if ((*iter)->GetDestroy())
		{
			Object* temp = (*iter);
			iter = m_Objects.erase(iter);
			SafeDelete(temp);
		}
		else
		{
			++iter;
		}
	}
}
void ObjectMgr::CollisionCheak(Object* obj, const std::string tag)
{
	for (auto& iter : m_Objects)
	{
		if (iter->m_Tag == tag)
		{
			RECT rc;
			if (IntersectRect(&rc, &obj->m_Collision, &iter->m_Collision))
			{
				obj->OnCollision(iter);
				iter->OnCollision(obj);
			}
		}
	}
}
// ObjectMgr.cpp
void ObjectMgr::ReleaseByTag(const std::wstring& tag) {
	// m_byTag[tag] 안의 객체들만 안전하게 파괴/해제
	auto it = m_byTag.find(tag);
	if (it == m_byTag.end()) return;

	auto& vec = it->second;
	for (auto* obj : vec) {
		// 프로젝트 규약에 맞춰 안전 파괴
		// 예: obj->SetDestroy(true); 또는 delete obj;
		// 여기서는 실제 구현에 맞춰 교체
		delete obj;
	}
	vec.clear();
}

void ObjectMgr::ReleaseExceptTags(const std::unordered_set<std::wstring>& keep) {
	for (auto& kv : m_byTag) {
		const auto& tag = kv.first;
		if (keep.find(tag) != keep.end()) continue; // 보존 태그는 스킵

		auto& vec = kv.second;
		for (auto* obj : vec) {
			delete obj; // 또는 SetDestroy(true)
		}
		vec.clear();
	}
}

void ObjectMgr::DeleteObject(std::string tag)
{
	for (auto& iter : m_Objects)
	{
		if (iter->m_Tag == tag)
		{
			iter->SetDestroy(true);
		}
	}
}

void ObjectMgr::Update(float deltaTime, float time)
{
	DeleteCheak();
	for (const auto& iter : m_Objects)
	{
		(iter)->Update(deltaTime, time);
	}
}

void ObjectMgr::Render()
{
	m_Objects.sort(stLISTsort());

	for (const auto& iter : m_Objects)
	{
		(iter)->Render();
	}
}

void ObjectMgr::AddObject(Object* obj, const std::string tag)
{
	m_Objects.push_back(obj);
	obj->SetTag(tag);
}

void ObjectMgr::RemoveObject(Object* obj)
{
	if (obj)
		obj->SetDestroy(true);
}