#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

#define ObjMgr ObjectMgr::GetInst()
class ObjectMgr : public Singleton<ObjectMgr>
{
private:
	std::unordered_map<std::wstring, std::vector<Object*>> m_byTag;
public:
	std::list<Object*> m_Objects;

	struct stLISTsort
	{
		bool operator() (const Object* pObject1, const Object* pObject2) const
		{
			if (pObject1->m_Layer < pObject2->m_Layer)
				return TRUE;

			return FALSE;
		}
	};
	// 특정 태그만 정리 (예: "UI"만 싹 정리)
	void ReleaseByTag(const std::wstring& tag);

	// 특정 태그만 보존하고 나머지 전부 정리 (예: {"UI_PERSIST"}만 살림)
	void ReleaseExceptTags(const std::unordered_set<std::wstring>& keep);

public:
	ObjectMgr();
	~ObjectMgr();

	void Release();
	void DeleteCheak();
	void Update(float deltaTime, float time);
	void Render();




public:
	void AddObject(Object* obj, const std::string tag);
	void RemoveObject(Object* obj);
	void CollisionCheak(Object* obj, const std::string tag);
	void DeleteObject(std::string tag);

};

