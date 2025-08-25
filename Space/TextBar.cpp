#include "stdafx.h"
#include "TextBar.h"
#include "TextMgr.h"
#include "TextTypeSfx.h"

TextBar::TextBar(int x, int y, float charDelay, float clearDelay)
    : x_(x), y_(y), charDelay_(charDelay), clearDelay_(clearDelay) {
}

TextBar::~TextBar() {}

void TextBar::SetFont(TextMgr* font) { font_ = font; }
void TextBar::SetTypeSfx(TextTypeSfx* s) { typeSfx_ = s; }

void TextBar::Push(const std::string& msg) {
    queue_.push(msg);
    // 현재 비어있다면 즉시 시작
    if (!active_ && !done_) BeginNextMessage();
}

void TextBar::BeginNextMessage() {
    if (queue_.empty()) {
        // 더 이상 출력할 문장이 없음
        active_ = false;
        done_ = false;
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
    // 글자 지연을 넘은 만큼 여러 글자를 한 번에 표시(프레임 드랍 대비)
    while (active_ && !done_ && acc_ >= charDelay_) {
        acc_ -= charDelay_;

        // 다음 글자 출력
        char ch = cur_[idx_++];
        visible_.push_back(ch);

        // 타자음(특정 기호/공백/IME 조합 제외 등은 TextTypeSfx 내부 정책)
        if (typeSfx_) typeSfx_->OnCharInput(ch);

        // 문장 완료 체크
        if (idx_ >= cur_.size()) {
            done_ = true;
            active_ = false;
            break;
        }
    }
}

void TextBar::Update(float t) {
    if (font_ == nullptr) return;

    if (!done_) {
        // 타이핑 진행 중
        StepType(dt);
    }
    else {
        // 출력 완료 → 일정 시간 후 자동 클리어
        clearAcc_ += dt;
        if (clearAcc_ >= clearDelay_) {
            // 화면에서 지우고 다음 메시지로
            visible_.clear();
            done_ = false;
            BeginNextMessage();
        }
    }
}

void TextBar::Render() {
    if (font_ == nullptr) return;
    // 비어있어도 공백 출력(텍스트바 위치 유지)
    font_->print(visible_, x_, y_);
}
