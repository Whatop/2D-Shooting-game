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
    for (auto* obj : m_Objects)
    {
        if (obj)
            obj->SetDestroy(true);
    }
}

// destroy 되었거나 null인 포인터를 태그 인덱스에서 제거
static void CompactVector(std::vector<Object*>& v)
{
    v.erase(std::remove_if(v.begin(), v.end(),
        [](Object* o)
        {
            return o == nullptr || o->GetDestroy();
        }),
        v.end());
}

void ObjectMgr::DeleteCheck()
{
    // 실제 소유 컨테이너에서 삭제
    for (auto it = m_Objects.begin(); it != m_Objects.end(); )
    {
        Object* obj = *it;

        if (obj == nullptr || obj->GetDestroy())
        {
            it = m_Objects.erase(it);
            SafeDelete(obj);
        }
        else
        {
            ++it;
        }
    }

    // 태그 인덱스 정리
    for (auto& kv : m_byTag)
        CompactVector(kv.second);
}

void ObjectMgr::CollisionCheck(Object* obj, const std::string& tag)
{
    if (!obj || obj->GetDestroy())
        return;

    for (auto* iter : m_Objects)
    {
        if (!iter) continue;
        if (iter == obj) continue;
        if (iter->GetDestroy()) continue;
        if (iter->m_Tag != tag) continue;

        RECT rc;
        if (IntersectRect(&rc, &obj->m_Collision, &iter->m_Collision))
        {
            obj->OnCollision(iter);
            iter->OnCollision(obj);
        }
    }
}

void ObjectMgr::ReleaseByTag(const std::string& tag)
{
    auto it = m_byTag.find(tag);
    if (it == m_byTag.end())
        return;

    for (auto* obj : it->second)
    {
        if (obj)
            obj->SetDestroy(true);
    }

    // 인덱스만 비움
    it->second.clear();
}

void ObjectMgr::ReleaseExceptTags(const std::unordered_set<std::string>& keep)
{
    for (auto& kv : m_byTag)
    {
        if (keep.find(kv.first) != keep.end())
            continue;

        for (auto* obj : kv.second)
        {
            if (obj)
                obj->SetDestroy(true);
        }

        kv.second.clear();
    }
}

void ObjectMgr::DeleteObject(const std::string& tag)
{
    for (auto* obj : m_Objects)
    {
        if (!obj) continue;

        if (obj->m_Tag == tag)
            obj->SetDestroy(true);
    }
}

void ObjectMgr::Update(float deltaTime, float time)
{
    // 이전 프레임에서 삭제 예약된 객체 정리
    DeleteCheck();

    for (auto* obj : m_Objects)
    {
        if (!obj) continue;
        if (obj->GetDestroy()) continue;

        obj->Update(deltaTime, time);
    }

    // 이번 프레임에서 삭제 예약된 객체 정리
    DeleteCheck();
}

void ObjectMgr::Render()
{
    m_Objects.sort(stLISTsort());

    for (auto* obj : m_Objects)
    {
        if (!obj) continue;
        if (obj->GetDestroy()) continue;

        obj->Render();
    }
}

void ObjectMgr::AddObject(Object* obj, const std::string& tag)
{
    if (!obj)
        return;

    obj->SetTag(tag);
    m_Objects.push_back(obj);
    m_byTag[tag].push_back(obj);
}

void ObjectMgr::RemoveObject(Object* obj)
{
    if (obj)
        obj->SetDestroy(true);
}