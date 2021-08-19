#include"stdafx.h"
#include"MainScene.h"
#include"StoreScene.h"

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
#if _DEBUG
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "wt", stdout);

#endif
	GameInfo->Init();
	GameInfo->RankInit();
	App::GetInst()->Init(1920, 1080, 1);
	SceneDirector::GetInst()->ChangeScene(new StoreScene());
	App::GetInst()->Run();
	return 0;
}