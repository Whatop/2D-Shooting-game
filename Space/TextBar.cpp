// TextBar.cpp
#include "stdafx.h"
#include "TextBar.h"
#include "TextMgr.h"
#include "TextTypeSfx.h"
#include <Windows.h>

static std::wstring FromUTF8(const std::string& s) {
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring w;
    if (n <= 0) return w;
    w.resize(n - 1); // exclude null
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    return w;
}

TextBar::TextBar(int x, int y, float charDelay, float clearDelay)
    : x_(x), y_(y), charDelay_(charDelay), clearDelay_(clearDelay) {}

void TextBar::SetFont(TextMgr* f) { font_ = f; }
void TextBar::SetTypeSfx(TextTypeSfx* s) { typeSfx_ = s; }

void TextBar::Push(const std::wstring& msg, bool sticky) {
    queue_.push(Item{ msg, sticky });
    // 비활성 상태면 바로 시작
    if (!active_ && !done_) BeginNextMessage();
}

void TextBar::PushUTF8(const std::string& utf8, bool sticky) {
    Push(FromUTF8(utf8), sticky);
}

void TextBar::BeginNextMessage() {
    if (queue_.empty()) {
        active_ = false;
        done_ = false;
        visible_.clear();
        return;
    }

    cur_ = queue_.front();
    queue_.pop();

    visible_.clear();
    idx_ = 0;
    acc_ = 0.f;
    clearAcc_ = 0.f;
    active_ = true;
    done_ = false;
}

void TextBar::StepType(float t) {
    acc_ += t;

    // 안전 장치: 남은 문자가 없으면 조기 종료
    if (!active_ || done_) return;

    while (acc_ >= charDelay_ && idx_ < cur_.text.size()) {
        acc_ -= charDelay_;

        const wchar_t ch = cur_.text[idx_++];
        visible_.push_back(ch);

        if (typeSfx_) {
            // 문자당 1회 호출 (필터링은 Sfx 쪽 정책)
            typeSfx_->OnCharInput('a'); // 필요 시 실제 문자로 변경 가능
        }

        // 마지막 문자 처리 후 종료
        if (idx_ >= cur_.text.size()) {
            done_ = true;
            active_ = false;
            break;
        }
    }
}

void TextBar::Update(float t) {
    if (!font_) return;

    // 타이핑 중
    if (!done_) {
        StepType(t);
        return;
    }

    // 완료 후 처리
    if (cur_.sticky) {
        // sticky면 자동 전환/클리어 안 함
        return;
    }

    clearAcc_ += t;
    if (clearAcc_ >= clearDelay_) {
        visible_.clear();
        done_ = false;
        BeginNextMessage();
    }
}

void TextBar::Render() {
    if (!font_) return;
    // DrawTextW 기반이면 줄바꿈(\n) 자동 처리
    font_->print(visible_, x_, y_);
}

void TextBar::ReleaseSticky() {
    if (cur_.sticky) {
        cur_.sticky = false; // 다음 Update에서 평소 로직으로 넘어감
    }
}

void TextBar::ClearAll() {
    while (!queue_.empty()) queue_.pop();
    visible_.clear();
    idx_ = 0;
    active_ = false;
    done_ = false;
    cur_ = Item{};
}
