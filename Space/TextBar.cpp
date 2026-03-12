// TextBar.cpp
#include "stdafx.h"
#include "TextBar.h"
#include "TextMgr.h"
#include "TextTypeSfx.h"
#include <Windows.h>

// UTF-8 문자열을 wide string으로 변환
static std::wstring FromUTF8(const std::string& s) {
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring w;
    if (n <= 0) return w;

    w.resize(n - 1); // null 문자 제외한 길이만 확보
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &w[0], n);
    return w;
}

// 생성자
TextBar::TextBar(int x, int y, float charDelay, float clearDelay)
    : x_(x), y_(y), charDelay_(charDelay), clearDelay_(clearDelay) {
}

// 사용할 폰트 지정
void TextBar::SetFont(TextMgr* f) { font_ = f; }

// 사용할 타자 효과음 객체 지정
void TextBar::SetTypeSfx(TextTypeSfx* s) { typeSfx_ = s; }

// wide 문자열 메시지 추가
void TextBar::Push(const std::wstring& msg, bool sticky) {
    queue_.push(Item{ msg, sticky });

    // 현재 아무 메시지도 진행 중이 아니면 바로 시작
    if (!active_ && !done_) BeginNextMessage();
}

// UTF-8 문자열 메시지 추가
void TextBar::PushUTF8(const std::string& utf8, bool sticky) {
    Push(FromUTF8(utf8), sticky);
}

// 다음 메시지를 현재 메시지로 가져와 타이핑 시작
void TextBar::BeginNextMessage() {
    if (queue_.empty()) {
        // 남은 메시지가 없으면 상태 초기화
        active_ = false;
        done_ = false;
        visible_.clear();
        return;
    }

    // 큐의 맨 앞 메시지를 현재 메시지로 꺼냄
    cur_ = queue_.front();
    queue_.pop();

    // 출력 상태 초기화
    visible_.clear();
    idx_ = 0;
    acc_ = 0.f;
    clearAcc_ = 0.f;
    active_ = true;
    done_ = false;
}

// 현재 메시지를 한 글자씩 진행
void TextBar::StepType(float t) {
    acc_ += t;

    // 이미 끝났거나 진행 중이 아니면 아무것도 안 함
    if (!active_ || done_) return;

    // 누적 시간이 charDelay를 넘을 때마다 글자 하나씩 출력
    while (acc_ >= charDelay_ && idx_ < cur_.text.size()) {
        acc_ -= charDelay_;

        const wchar_t ch = cur_.text[idx_++];
        visible_.push_back(ch);

        // 효과음 객체가 있으면 문자당 1회 호출
        if (typeSfx_) {
            // 현재는 실제 문자 대신 고정값 'a'를 넘김
            // 효과음 정책이 TextTypeSfx 쪽에 있는 구조
            typeSfx_->OnCharInput('a');
        }

        // 마지막 문자까지 출력했으면 종료 처리
        if (idx_ >= cur_.text.size()) {
            done_ = true;
            active_ = false;
            break;
        }
    }
}

// 매 프레임 업데이트
void TextBar::Update(float t) {
    // 폰트가 없으면 출력할 수 없으므로 중단
    if (!font_) return;

    // 아직 현재 문장이 끝나지 않았으면 타이핑 진행
    if (!done_) {
        StepType(t);
        return;
    }

    // 완료된 뒤 sticky 메시지면 자동으로 안 지움
    if (cur_.sticky) {
        return;
    }

    // sticky가 아니면 일정 시간 뒤 지우고 다음 메시지 시작
    clearAcc_ += t;
    if (clearAcc_ >= clearDelay_) {
        visible_.clear();
        done_ = false;
        BeginNextMessage();
    }
}

// 현재 보이는 문자열 렌더
void TextBar::Render() {
    if (!font_) return;

    // DrawTextW 계열이면 줄바꿈(\n) 자동 처리 가능
    font_->print(visible_, x_, y_);
}

// sticky 메시지를 수동으로 해제
void TextBar::ReleaseSticky() {
    if (cur_.sticky) {
        cur_.sticky = false; // 다음 Update부터 일반 메시지처럼 진행
    }
}

// 전체 대화 상태 초기화
void TextBar::ClearAll() {
    while (!queue_.empty()) queue_.pop();

    visible_.clear();
    idx_ = 0;
    active_ = false;
    done_ = false;
    cur_ = Item{};
}