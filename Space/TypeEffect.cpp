#include "stdafx.h"
#include "SoundMgr.h"
#include "TypeEffect.h"
#include "AudioLibrary.h"

// local helper to avoid <algorithm> / Windows max macro conflicts
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

void TypeEffect::Update(float t) {             // <-- 파라미터 이름 t
    if (idx_ >= (int)cur_.size()) return;

    acc_ += t;                                  // <-- 내부도 t 사용
    sfxCooldown_ -= t;

    const float perChar = 1.0f / Maxf(1.0f, cps_);
    while (acc_ >= perChar && idx_ < (int)cur_.size()) {
        acc_ -= perChar;

        char ch = cur_[idx_++];
        bool isPunct = (ch == '.' || ch == ',' || ch == '!' || ch == '?' || ch == ' ');

        if (!isPunct && typeSfx_ && sfxCooldown_ <= 0.f) {
            typeSfx_->stop();
            typeSfx_->play();
            sfxCooldown_ = sfxInterval_;
        }
    }
}
