#pragma once
#include <string>
#include "AudioLibrary.h"

// 문자열을 한 글자씩 출력하면서, 일정 간격으로 타자 효과음을 재생하는 클래스
class TypeEffect {
public:
    // 사용할 타자 효과음 종류 설정
    void SetTypeSfx(TypeSfx key);

    // 출력할 문자열과 초당 출력 글자 수 설정
    void Start(const std::string& text, float cps);

    // 매 프레임 호출하여 타자 진행
    // t는 누적시간이 아니라 프레임 델타타임(dt)로 쓰는 구조
    void Update(float t);

    // 현재까지 출력된 글자 수 반환
    int TypedCount() const { return idx_; }

    // 현재 화면에 보여줄 문자열 반환
    std::string Visible() const { return cur_.substr(0, idx_); }

    // 전체 출력 완료 여부
    bool Done() const { return idx_ >= (int)cur_.size(); }

private:
    std::string cur_;   // 전체 문자열
    int   idx_ = 0;     // 현재까지 출력한 인덱스
    float cps_ = 20.f;  // 초당 출력 글자 수
    float acc_ = 0.f;   // 글자 출력 누적 시간

    TypeSfx   typeKey_ = TypeSfx::Human;
    SoundMgr* typeSfx_ = nullptr;

    float sfxInterval_ = 0.02f; // 효과음 최소 간격
    float sfxCooldown_ = 0.f;   // 다음 효과음 재생 가능까지 남은 시간
};