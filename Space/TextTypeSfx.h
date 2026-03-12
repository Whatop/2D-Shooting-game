#pragma once
// 한 글자 입력(확정)마다 효과음을 재생하되, 공백/특정기호/IME 조합 중은 제외한다.

#include <unordered_set>
#include <string>
#include <random>
#include <chrono>

class SoundMgr; // 효과음 재생용 사운드 객체

class TextTypeSfx {
public:
    // sfx           : 재생할 타자 효과음
    // minIntervalMs : 너무 빠른 연속 재생을 막기 위한 최소 간격(ms)
    // pitchJitter   : 향후 피치 랜덤값 적용용
    TextTypeSfx(SoundMgr* sfx, int minIntervalMs = 30, float pitchJitter = 0.0f);

    // 문자 1개 입력 시 호출
    void OnCharInput(char ch);

    // 문자열 여러 글자 입력 시 호출
    void OnTextInput(const std::string& s);

    // 제외 문자 설정
    void SetExcluded(const std::unordered_set<char>& chars);
    void AddExcluded(char ch);
    void RemoveExcluded(char ch);
    void ClearExcluded();

    // 공백 제외 여부
    void SetExcludeWhitespace(bool exclude);

    // 최소 재생 간격 설정
    void SetMinIntervalMs(int ms);

    // 피치 랜덤 범위 설정
    void SetPitchJitter(float r);

    // IME 조합 중 여부 설정
    void SetComposition(bool composing);

private:
    using Clock = std::chrono::steady_clock;

    // 현재 문자가 재생 가능한 문자인지 판정
    bool ShouldPlay(char ch) const;

    // 피치 랜덤값 생성
    float RandPitch();

private:
    SoundMgr* sfx_ = nullptr;
    std::unordered_set<char> excluded_;

    bool excludeWhitespace_ = true; // 기본적으로 공백은 제외
    bool composing_ = false;        // 한글 조합 중에는 재생 금지

    int minIntervalMs_ = 30;
    float pitchJitter_ = 0.0f;

    std::mt19937 rng_;
    Clock::time_point last_;
};