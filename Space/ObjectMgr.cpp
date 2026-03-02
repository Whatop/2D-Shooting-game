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

static void CompactVector(std::vector<Object*>& v)
{
	v.erase(std::remove_if(v.begin(), v.end(),
		[](Object* o) { return o == nullptr || o->GetDestroy(); }), v.end());
}

void ObjectMgr::DeleteCheak()
{
	for (auto it = m_Objects.begin(); it != m_Objects.end(); )
	{
		Object* obj = *it;
		if (obj->GetDestroy())
		{
			it = m_Objects.erase(it);
			SafeDelete(obj);
		}
		else ++it;
	}

	// ✅ 인덱스 청소
	for (auto& kv : m_byTag) CompactVector(kv.second);
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
void ObjectMgr::ReleaseByTag(const std::string& tag)
{
    auto it = m_byTag.find(tag);
    if (it == m_byTag.end()) return;

    for (auto* obj : it->second)
        if (obj) obj->SetDestroy(true);   // 소유권은 m_Objects 쪽에서만

    it->second.clear(); // 인덱스만 비움
}

void ObjectMgr::ReleaseExceptTags(const std::unordered_set<std::string>& keep)
{
    for (auto& kv : m_byTag)
    {
        if (keep.find(kv.first) != keep.end()) continue;

        for (auto* obj : kv.second)
            if (obj) obj->SetDestroy(true);

        kv.second.clear();
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
	m_byTag[tag].push_back(obj); // 인덱스(비소유)
}

void ObjectMgr::RemoveObject(Object* obj)
{
	if (obj)
		obj->SetDestroy(true);
}