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

    w.resize(n - 1);                        // 널 제외 크기 확보
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n); // &w[0]로 LPWSTR 보장
    return w;
}

TextBar::TextBar(int x, int y, float charDelay, float clearDelay)
    : x_(x), y_(y), charDelay_(charDelay), clearDelay_(clearDelay) {}

void TextBar::SetFont(TextMgr* f) { font_ = f; }
void TextBar::SetTypeSfx(TextTypeSfx* s) { typeSfx_ = s; }

void TextBar::Push(const std::wstring& msg, bool sticky) {
    queue_.push(Item{ msg, sticky });
    if (!active_ && !done_) BeginNextMessage();
}

void TextBar::PushUTF8(const std::string& utf8, bool sticky) {
    Push(FromUTF8(utf8), sticky);
}

void TextBar::BeginNextMessage() {
    if (queue_.empty()) {
        active_ = false; done_ = false;
        visible_.clear();
        return;
    }
    cur_ = queue_.front(); queue_.pop();

    visible_.clear();
    idx_ = 0;
    acc_ = 0.f;
    clearAcc_ = 0.f;
    active_ = true;
    done_ = false;
}

void TextBar::StepType(float t) {
    acc_ += dt;
    while (active_ && !done_ && acc_ >= charDelay_) {
        acc_ -= charDelay_;

        const wchar_t ch = cur_.text[idx_++];
        visible_.push_back(ch);

        if (typeSfx_) {
            // 필터는 Sfx 쪽 정책으로 처리. 문자당 1회만 호출
            typeSfx_->OnCharInput('a');
        }

        if (idx_ >= cur_.text.size()) {
            done_ = true;
            active_ = false;
            break;
        }
    }
}

void TextBar::Update(float t) {
    if (!font_) return;

    if (!done_) {
        StepType(dt);
        return;
    }

    // 여기서부터 '완료 후' 처리
    if (cur_.sticky) {
        // sticky면 자동 클리어/다음 메시지로 넘어가지 않음
        return;
    }

    clearAcc_ += dt;
    if (clearAcc_ >= clearDelay_) {
        visible_.clear();
        done_ = false;
        BeginNextMessage();
    }
}

void TextBar::Render() {
    if (!font_) return;
    // DrawTextW 기반이면 \n 줄바꿈이 자동 처리됨
    font_->print(visible_, x_, y_);
}

void TextBar::ReleaseSticky() {
    // 현재 표시 중인 게 sticky일 때만 해제
    if (cur_.sticky) {
        cur_.sticky = false;
        // 다음 Update에서 평소 로직으로 클리어/전환됨
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
