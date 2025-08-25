// TextBar.h
#pragma once
#include <string>
#include <queue>

class TextMgr;
class TextTypeSfx;

class TextBar {
public:
    TextBar(int x, int y, float charDelay = 0.03f, float clearDelay = 1.0f);

    // 변경: sticky(기본 false). true면 자동 클리어/다음 전환 막음
    void Push(const std::wstring& msg, bool sticky = false);

    // 편의: UTF-8 문자열로도 넣고 싶을 때
    void PushUTF8(const std::string& utf8, bool sticky = false);

    void Update( float Time);
    void Render();

    void SetFont(TextMgr* font);
    void SetTypeSfx(TextTypeSfx* sfx);

    // 수동 해제용(필요 시 사용): 현재 sticky 메시지를 해제하고 다음으로
    void ReleaseSticky();
    // 전체 비우기(선택): 현재/대기 메시지 모두 제거
    void ClearAll();

private:
    struct Item {
        std::wstring text;
        bool sticky = false;
    };

    void BeginNextMessage();
    void StepType(float t);

    std::queue<Item> queue_;
    Item cur_;                    // 현재 표시 중 메시지(텍스트+sticky)
    std::wstring visible_;        // 화면에 보이는 누적 텍스트
    size_t idx_ = 0;

    bool active_ = false;         // 타이핑 진행 중
    bool done_ = false;           // 한 문장 타이핑 완료

    float charDelay_;
    float clearDelay_;
    float acc_ = 0.f;
    float clearAcc_ = 0.f;

    int x_, y_;
    TextMgr* font_ = nullptr;
    TextTypeSfx* typeSfx_ = nullptr;
};
