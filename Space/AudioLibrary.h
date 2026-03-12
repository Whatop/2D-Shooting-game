#pragma once

#include <map>
#include "SoundMgr.h"

// 대사/효과음 종류 구분용 enum
enum class TypeSfx
{
    Human,    // 사람 목소리 타입
    Robot,    // 기계/로봇 느낌 타입
    Monster,  // 몬스터 타입
    Flower,   // 꽃/특수 타입
    Silent    // 무음 처리용
};

// AudioLibrary
// 효과음 종류(TypeSfx)별로 SoundMgr를 하나씩 보관하는 정적 라이브러리 클래스
class AudioLibrary
{
public:
    // 사운드 라이브러리 초기화(사운드 로드)
    static void Init();

    // 수정:
    // 정리 함수 추가
    // Init()에서 new로 만든 SoundMgr들을 해제하기 위해 필요함
    static void Release();

    // 타입에 맞는 SoundMgr 반환
    static SoundMgr* Get(TypeSfx key);

private:
    // TypeSfx를 키로 SoundMgr 포인터를 저장
    static std::map<TypeSfx, SoundMgr*> sfxMap;
};