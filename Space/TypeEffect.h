#pragma once
#include <string>
#include "AudioLibrary.h"

class TypeEffect {
public:
    void SetTypeSfx(TypeSfx key);
    void Start(const std::string& text, float cps);
    void Update(float t);               // <-- 여기! dt가 아니라 t로 고정

    int  TypedCount() const { return idx_; }
    std::string Visible() const { return cur_.substr(0, idx_); }
    bool Done() const { return idx_ >= (int)cur_.size(); }

private:
    std::string cur_;
    int   idx_ = 0;
    float cps_ = 20.f;
    float acc_ = 0.f;

    TypeSfx   typeKey_ = TypeSfx::Human;
    SoundMgr* typeSfx_ = nullptr;
    float     sfxInterval_ = 0.02f;
    float     sfxCooldown_ = 0.f;
};
