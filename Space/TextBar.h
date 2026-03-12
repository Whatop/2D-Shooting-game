// TextBar.h
#pragma once
#include <string>
#include <queue>

class TextMgr;
class TextTypeSfx;

// 텍스트를 한 글자씩 출력해주는 대사 바 클래스
class TextBar {
public:
    // x, y : 출력 위치
    // charDelay : 글자 하나가 나타나는 간격
    // clearDelay : 한 문장이 끝난 뒤 자동으로 지워지기까지의 시간
    TextBar(int x, int y, float charDelay = 0.03f, float clearDelay = 1.0f);

    // 대사 추가
    // sticky=true면 자동 클리어/다음 메시지 진행을 막음
    void Push(const std::wstring& msg, bool sticky = false);

    // UTF-8 문자열 입력 편의용
    void PushUTF8(const std::string& utf8, bool sticky = false);

    // 매 프레임 업데이트
    void Update(float t);

    // 현재 보이는 텍스트 렌더
    void Render();

    // 사용할 폰트 객체 지정
    void SetFont(TextMgr* font);

    // 문자 입력 효과음 객체 지정
    void SetTypeSfx(TextTypeSfx* sfx);

    // 현재 sticky 메시지를 수동 해제하고 다음 진행 가능하게 만듦
    void ReleaseSticky();

    // 전체 대사 큐와 현재 상태를 모두 비움
    void ClearAll();

private:
    // 큐에 들어갈 메시지 1개 단위
    struct Item {
        std::wstring text; // 실제 텍스트
        bool sticky = false; // 자동 전환 막을지 여부
    };

    // 다음 메시지를 현재 메시지로 시작
    void BeginNextMessage();

    // 한 글자씩 타이핑 진행
    void StepType(float t);

    std::queue<Item> queue_; // 대기 중인 메시지 큐
    Item cur_;               // 현재 출력 중인 메시지
    std::wstring visible_;   // 현재 화면에 보이는 누적 문자열
    size_t idx_ = 0;         // 현재 몇 글자까지 출력했는지 인덱스

    bool active_ = false;    // 타이핑 진행 중인지
    bool done_ = false;      // 현재 한 문장이 끝났는지

    float charDelay_;        // 글자 간 출력 딜레이
    float clearDelay_;       // 완료 후 자동 클리어 딜레이
    float acc_ = 0.f;        // 글자 출력 누적 시간
    float clearAcc_ = 0.f;   // 완료 후 경과 시간

    int x_, y_;              // 출력 좌표
    TextMgr* font_ = nullptr;        // 출력용 폰트
    TextTypeSfx* typeSfx_ = nullptr; // 타자 효과음 처리 객체
};