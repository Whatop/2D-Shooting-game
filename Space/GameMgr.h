#pragma once
#define GameInfo GameMgr::GetInst()
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

class GameMgr : public Singleton<GameMgr>
{
	Object* PlayerInfo;
public:
	GameMgr();
	~GameMgr();


	bool m_isCreateUI;
	bool m_DebugMode;
	bool m_isCreatePlayer;

	void Init();
	void Release();

	void CreateUI();
	void ReleaseUI();

	void CreatePlayer();
	void PlayerDeath();
	void PlayerUpdate(Object* obj) { PlayerInfo = obj; }
	Object*GetPlayerInfo() { return PlayerInfo; }
	

	void Update();
	void Render();

};

