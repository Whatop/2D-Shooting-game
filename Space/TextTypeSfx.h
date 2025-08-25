#pragma once
// 한 글자 입력(확정)마다 효과음을 재생하되, 공백/특정기호/IME 조합 중은 제외.
// 주의: 주석은 한국어, 이모지 금지

#include <unordered_set>
#include <string>
#include <random>
#include <chrono>

class SoundMgr; // FMOD 래퍼(프로젝트 제공)

class TextTypeSfx {
public:
    // sfx: 사전 로딩된 단발 SFX(루프 꺼짐), minIntervalMs: 최소 재생 간격(ms)
    // pitchJitter: 피치 변조 폭(0이면 미사용, SoundMgr에 SetPitch 추가 시 반영)
    TextTypeSfx(SoundMgr* sfx, int minIntervalMs = 30, float pitchJitter = 0.0f);

    // 한 글자 확정(입력) 시 호출
    void OnCharInput(char ch);
    // 여러 글자(붙여넣기 등) 확정 시 호출
    void OnTextInput(const std::string& s);

    // 제외 문자 집합 제어
    void SetExcluded(const std::unordered_set<char>& chars);
    void AddExcluded(char ch);
    void RemoveExcluded(char ch);
    void ClearExcluded();

    void SetExcludeWhitespace(bool exclude);
    void SetMinIntervalMs(int ms);
    void SetPitchJitter(float r);     // SoundMgr에 피치 API가 있을 때만 의미 있음
    void SetComposition(bool composing); // IME 조합(true) 중에는 재생 금지

private:
    using Clock = std::chrono::steady_clock;

    bool ShouldPlay(char ch) const;
    float RandPitch();

    SoundMgr* sfx_ = nullptr;
    std::unordered_set<char> excluded_;
    bool excludeWhitespace_ = true;   // 기본: 공백류 제외
    bool composing_ = false;          // IME 조합 중 여부
    int  minIntervalMs_ = 30;
    float pitchJitter_ = 0.0f;

    std::mt19937 rng_;
    Clock::time_point last_;
};
