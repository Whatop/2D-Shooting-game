#include "stdafx.h"
#include "TextTypeSfx.h"
#include "SoundMgr.h"
#include "AudioLibrary.h"

// AudioLibrary.cpp
std::map<TypeSfx, SoundMgr*> AudioLibrary::sfxMap;

void AudioLibrary::Init() {
    // loop=false, stream=true 권장(짧은 효과음은 stream 생략 가능)
    sfxMap[TypeSfx::Human] = new SoundMgr("Sound/SND_TXT2.wav", false);
    sfxMap[TypeSfx::Robot] = new SoundMgr("Sound/snd_txtpap.wav", false);
    sfxMap[TypeSfx::Monster] = new SoundMgr("Sound/snd_txtpap.wav", false);
    sfxMap[TypeSfx::Flower] = new SoundMgr("Sound/snd_txtsans2.wav", false);
    sfxMap[TypeSfx::Silent] = nullptr; // 무음
}
SoundMgr* AudioLibrary::Get(TypeSfx key) {
    auto it = sfxMap.find(key);
    return (it == sfxMap.end()) ? nullptr : it->second;
}
