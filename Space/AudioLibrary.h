#pragma once
// TypeSfx.h
enum class TypeSfx {
    Human, Robot, Monster, Flower, Silent
};

// AudioLibrary.h
#include <map>
class AudioLibrary {
public:
    static void Init();   // 사운드 로드
    static SoundMgr* Get(TypeSfx key);
private:
    static std::map<TypeSfx, SoundMgr*> sfxMap;
};
