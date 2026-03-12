#include "stdafx.h"
#include "AudioLibrary.h"

// 정적 멤버 정의
std::map<TypeSfx, SoundMgr*> AudioLibrary::sfxMap;

void AudioLibrary::Init()
{
    // 수정:
    // Init이 여러 번 호출될 수 있으므로
    // 기존에 이미 로드된 사운드가 있으면 먼저 정리
    Release();

    // 효과음 타입별 사운드 로드
    // 두 번째 인자는 루프 여부(false면 반복 재생 안 함)로 보임
    sfxMap[TypeSfx::Human] = new SoundMgr("Sound/SND_TXT2.wav", false);
    sfxMap[TypeSfx::Robot] = new SoundMgr("Sound/snd_txtpap.wav", false);
    sfxMap[TypeSfx::Monster] = new SoundMgr("Sound/snd_txtpap.wav", false);
    sfxMap[TypeSfx::Flower] = new SoundMgr("Sound/snd_txtsans2.wav", false);

    // Silent는 실제 사운드 객체를 만들지 않고 nullptr로 저장
    // Get()에서 받아서 nullptr 체크하면 무음 처리 가능
    sfxMap[TypeSfx::Silent] = nullptr;
}

void AudioLibrary::Release()
{
    // 수정:
    // Init에서 new로 만든 SoundMgr들을 전부 정리
    for (auto& pair : sfxMap)
    {
        // Silent는 nullptr일 수 있으므로 SafeDelete 스타일로 체크
        if (pair.second != nullptr)
        {
            delete pair.second;
            pair.second = nullptr;
        }
    }

    sfxMap.clear();
}

SoundMgr* AudioLibrary::Get(TypeSfx key)
{
    auto it = sfxMap.find(key);

    // 해당 타입이 없으면 nullptr 반환
    return (it == sfxMap.end()) ? nullptr : it->second;
}