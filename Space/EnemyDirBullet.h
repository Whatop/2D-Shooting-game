#pragma once
class EnemyDirBullet : public Object
{
	Sprite* m_Bullet;
	Vec2 m_Dire;
	float m_Speed;
public:
	EnemyDirBullet(Vec2 Pos,Vec2 Dir);
	~EnemyDirBullet();
	
	void Update(float deltaTime, float Time);
	void Render();
	
	void OnCollision(Object* obj);
	
	void Move();
};

