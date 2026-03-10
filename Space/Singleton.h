#pragma once

// 템플릿 기반 싱글톤 클래스
template <class T>
class Singleton
{
protected:
	static T* m_Instance;; // 단 하나의 인스턴스 포인터

protected:
	Singleton() {}
	virtual ~Singleton() {}
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T* GetInst()
	{
		if (m_Instance == nullptr)
			m_Instance = new T();

		return m_Instance;
	}

	// 인스턴스 삭제
	static void ReleaseInst()
	{
		if (m_Instance != nullptr)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}
};

// 정적 멤버 초기화
template<class T>
T* Singleton<T>::m_Instance = 0;