#pragma once
class Object
{
	float DestroyTime;
public:
	Object* m_Parent;
	Matrix m_wMat;
	Vec2 m_Position;
	Vec2 m_Scale;
	Vec2 m_Size;
	Vec2 m_RotationCenter;
	Vec2 m_ScaleCenter;

	float m_Radius;
	float m_Rotation;

	float m_Atk;

	bool m_Destroy;
	RECT m_Collision;

	bool m_Visible;

	int m_Layer;
	std::string m_Tag;

public:
	Object();
	virtual ~Object();


protected:
	Matrix GetMatrix();

public:
	virtual void Update(float deltaTime, float time);
	virtual void Render();
	virtual void OnCollision(Object* other);

public:
	void Translate(float x, float y);
	void SetScale(float x, float y);
	void SetPosition(Vec2 pos);
	void SetPosition(float x, float y);
	void Rotate(float r);
	void SetDestroy(bool destroy) { m_Destroy = destroy; }
	void SetTag(const std::string tag);
	void SetParent(Object* obj);
	void DelayDestroy(Object* obj,float destroyTime);
	inline float RandRange(float a, float b) {
		return a + (b - a) * (float(rand()) / float(RAND_MAX));
	}
public:
	bool GetDestroy() { return m_Destroy; }
};