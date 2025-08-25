#pragma once
// 간단 텍스트바: 메시지 순차 출력(타자 효과) + 완료 후 자동 클리어
// 주석은 한국어, 이모지 금지

#include <string>
#include <queue>

class TextMgr;
class TextTypeSfx; // 이전에 만든 타자음 유틸

class TextBar {
public:
    // charDelay: 글자당 지연(초), clearDelay: 문장 완료 후 클리어 대기(초)
    TextBar(int x, int y, float charDelay = 0.03f, float clearDelay = 1.0f);
    ~TextBar();

    // 메시지를 큐에 추가
    void Push(const std::string& msg);

    // 매 프레임 갱신/렌더
    void Update(float t);
    void Render();

    // 외부 연결(폰트/사운드)
    void SetFont(TextMgr* font);           // 필수: TextMgr (폰트 인스턴스)
    void SetTypeSfx(TextTypeSfx* sfx);     // 선택: 타자음 유틸

private:
    // 내부 상태
    std::queue<std::string> queue_;
    std::string cur_, visible_;
    size_t idx_ = 0;
    bool active_ = false;      // 메시지 출력 중
    bool done_ = false;        // 한 문장 출력 완료

    // 타이밍
    float charDelay_;          // 글자당 지연
    float acc_ = 0.f;          // 글자 출력 누적 시간
    float clearDelay_;         // 완료 후 클리어까지 대기
    float clearAcc_ = 0.f;

    // 렌더 위치
    int x_, y_;

    TextMgr* font_ = nullptr;          // 외부에서 주입
    TextTypeSfx* typeSfx_ = nullptr;   // 외부에서 주입

    void BeginNextMessage();           // 다음 메시지 시작
    void StepType(float t);           // 글자 한 글자씩 진행
};
