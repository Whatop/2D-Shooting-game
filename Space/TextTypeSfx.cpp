#include "stdafx.h"
#include "TextTypeSfx.h"
#include "SoundMgr.h"
#include <cctype>

TextTypeSfx::TextTypeSfx(SoundMgr* sfx, int minIntervalMs, float pitchJitter)
    : sfx_(sfx),
    excludeWhitespace_(true),
    composing_(false),
    minIntervalMs_(minIntervalMs),
    pitchJitter_(pitchJitter),
    rng_(std::random_device{}()),
    last_(Clock::now())
{
    // 기본 제외 문자: 필요 시 자유롭게 수정
    const char defaults[] = {
        '!', '?', '.', ',', ':', ';', '"', '\'', '-', '_',
        '(', ')', '[', ']', '{', '}', '/', '\\', '|', '*',
        '+', '=', '<', '>'
    };
    for (char c : defaults) excluded_.insert(c);
}

void TextTypeSfx::OnCharInput(char ch)
{
    if (!sfx_) return;
    if (!ShouldPlay(ch)) return;

    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();
    if (elapsed < minIntervalMs_) return;
    last_ = now;

    // 사운드 재생(단발). SoundMgr는 내부에서 m_channel를 보관
    sfx_->play();

    // 선택: SoundMgr에 SetPitch(float) 메서드를 추가했다면 피치 난수 적용
    // const float pitch = 1.0f + RandPitch();
    // sfx_->SetPitch(pitch);
}

void TextTypeSfx::OnTextInput(const std::string& s)
{
    for (unsigned char c : s)
        OnCharInput(static_cast<char>(c));
}

bool TextTypeSfx::ShouldPlay(char ch) const
{
    if (composing_) return false; // IME 조합 중에는 무음
    const unsigned char u = static_cast<unsigned char>(ch);
    if (excludeWhitespace_ && std::isspace(u)) return false; // 공백류 제외
    if (excluded_.count(ch)) return false; // 등록된 기호 제외
    return true;
}

float TextTypeSfx::RandPitch()
{
    if (pitchJitter_ <= 0.f) return 0.f;
    std::uniform_real_distribution<float> d(-pitchJitter_, pitchJitter_);
    return d(rng_);
}

void TextTypeSfx::SetExcluded(const std::unordered_set<char>& chars)
{
    excluded_ = chars;
}

void TextTypeSfx::AddExcluded(char ch)
{
    excluded_.insert(ch);
}

void TextTypeSfx::RemoveExcluded(char ch)
{
    excluded_.erase(ch);
}

void TextTypeSfx::ClearExcluded()
{
    excluded_.clear();
}

void TextTypeSfx::SetExcludeWhitespace(bool exclude)
{
    excludeWhitespace_ = exclude;
}

void TextTypeSfx::SetMinIntervalMs(int ms)
{
    minIntervalMs_ = ms;
}

void TextTypeSfx::SetPitchJitter(float r)
{
    pitchJitter_ = r;
}

void TextTypeSfx::SetComposition(bool composing)
{
    composing_ = composing;
}
