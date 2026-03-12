#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

#define ObjMgr ObjectMgr::GetInst()

class ObjectMgr : public Singleton<ObjectMgr>
{
private:
    // 태그별 빠른 접근용 인덱스
    // 실제 소유권은 m_Objects가 가짐
    std::unordered_map<std::string, std::vector<Object*>> m_byTag;

public:
    // 실제 오브젝트를 소유하는 컨테이너
    std::list<Object*> m_Objects;

    struct stLISTsort
    {
        bool operator()(const Object* pObject1, const Object* pObject2) const
        {
            return pObject1->m_Layer < pObject2->m_Layer;
        }
    };

    // 특정 태그만 정리
    void ReleaseByTag(const std::string& tag);

    // keep에 포함된 태그만 남기고 나머지는 정리
    void ReleaseExceptTags(const std::unordered_set<std::string>& keep);

public:
    ObjectMgr();
    ~ObjectMgr();

    void Release();
    void DeleteCheck();
    void Update(float deltaTime, float time);
    void Render();

public:
    void AddObject(Object* obj, const std::string& tag);
    void RemoveObject(Object* obj);
    void CollisionCheck(Object* obj, const std::string& tag);
    void DeleteObject(const std::string& tag);
};