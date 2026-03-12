#include "stdafx.h"
#include "SoundMgr.h"
#include "TypeEffect.h"
#include "AudioLibrary.h"

// Windows 매크로 충돌 피하려고 직접 만든 max 함수
static inline float Maxf(float a, float b) { return (a > b) ? a : b; }

void TypeEffect::SetTypeSfx(TypeSfx key) {
    typeKey_ = key;
    typeSfx_ = AudioLibrary::Get(key);
}

void TypeEffect::Start(const std::string& text, float cps) {
    cur_ = text;
    cps_ = cps;
    acc_ = 0.f;
    idx_ = 0;
    sfxCooldown_ = 0.f;
}

void TypeEffect::Update(float t) {
    // 이미 전부 출력했으면 종료
    if (idx_ >= (int)cur_.size()) return;

    // 프레임 dt 누적
    acc_ += t;
    sfxCooldown_ -= t;

    // cps가 0 이하로 들어와도 최소 1로 처리
    const float perChar = 1.0f / Maxf(1.0f, cps_);

    while (acc_ >= perChar && idx_ < (int)cur_.size()) {
        acc_ -= perChar;

        char ch = cur_[idx_++];
        bool isPunct = (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == ' ');

        // 문장부호/공백이 아닐 때만 타자 효과음 재생
        if (!isPunct && typeSfx_ && sfxCooldown_ <= 0.f) {
            typeSfx_->play();
            sfxCooldown_ = sfxInterval_;
        }
    }
}