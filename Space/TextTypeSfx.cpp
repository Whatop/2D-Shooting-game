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
    // 기본 제외 문자 등록
    const char defaults[] = {
        '!', '?', '.', ',', ':', ';', '"', '\'', '-', '_',
        '(', ')', '[', ']', '{', '}', '/', '\\', '|', '*',
        '+', '=', '<', '>'
    };

    for (char c : defaults)
        excluded_.insert(c);
}

void TextTypeSfx::OnCharInput(char ch)
{
    // 사운드가 없으면 종료
    if (!sfx_) return;

    // 현재 문자가 재생 대상이 아니면 종료
    if (!ShouldPlay(ch)) return;

    // 최소 재생 간격 체크
    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();
    if (elapsed < minIntervalMs_) return;

    last_ = now;

    // 효과음 재생
	sfx_->volumeSetting(0.5f); // 볼륨 조절 (필요에 따라 조정)

    sfx_->play();

    // 추후 SoundMgr에 피치 조절 함수가 생기면 사용 가능
    // const float pitch = 1.0f + RandPitch();
    // sfx_->SetPitch(pitch);
}

void TextTypeSfx::OnTextInput(const std::string& s)
{
    // 문자열의 각 글자에 대해 동일 처리
    for (unsigned char c : s)
        OnCharInput(static_cast<char>(c));
}

bool TextTypeSfx::ShouldPlay(char ch) const
{
    // 한글 조합 중이면 재생하지 않음
    if (composing_) return false;

    const unsigned char u = static_cast<unsigned char>(ch);

    // 공백 문자 제외
    if (excludeWhitespace_ && std::isspace(u)) return false;

    // 제외 문자 목록에 있으면 재생하지 않음
    if (excluded_.count(ch)) return false;

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