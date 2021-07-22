#pragma once
#define GM GameMgr::GetInst()

class GameMgr : public Singleton<GameMgr>
{
	Vec2 PlayerPosition;

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
	void PlayerPosUpdate(Vec2 Pos) { PlayerPosition = Pos; }
	Vec2 GetPlayerPos() { return PlayerPosition; }

	void Update();
	void Render();

};

