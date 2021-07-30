#pragma once
#define GameInfo GameMgr::GetInst()
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

class GameMgr : public Singleton<GameMgr>
{
	Object* PlayerInfo;

	float MaxHp, Hp;
	float BossMaxHp, BossHp;
public:
	GameMgr();
	~GameMgr();

	bool m_isCreateUI;
	bool m_DebugMode;
	bool AutoCamera;
	bool m_isCreatePlayer;
	bool CameraStop;
	void Init();
	void Release();

	void CreateUI();
	void ReleaseUI();

	void CreatePlayer();
	void PlayerDeath();
	Object*GetPlayerInfo() { return PlayerInfo; }
	void PlayerUpdate(Object* obj) { PlayerInfo = obj; }
	void PlayerHpUpdate(float maxhp,float hp) { MaxHp = maxhp, Hp = hp; }
	void BossHpUpdate(float maxhp,float hp) { BossMaxHp = maxhp, BossHp = hp; }
	float GetPlayerMaxHp() { return MaxHp; }
	float GetPlayerHp() { return Hp; }
	float GetBossMaxHp() { return BossMaxHp; }
	float GetBossHp() { return BossHp; }
	void Update();
	void Render();

};

