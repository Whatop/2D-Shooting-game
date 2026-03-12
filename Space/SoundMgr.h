#pragma once

#ifndef _CSOUND_H_
#define _CSOUND_H_

// 개별 사운드 볼륨 / 마스터 볼륨에 사용할 범위 상수
#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f
#define SOUND_MASTER_STEP 0.05f

// FMOD 사운드 하나를 관리하는 클래스
// 현재 구조상 여러 개 생성해서 효과음/BGM 각각 들고 쓰는 방식
class SoundMgr : public Singleton<SoundMgr>
{
private:
    // FMOD 시스템은 전체에서 하나만 공유
    static FMOD_SYSTEM* g_sound_system;

    FMOD_SOUND* m_sound;     // 로드된 사운드 리소스
    FMOD_CHANNEL* m_channel; // 현재 재생 중인 채널

    float m_volume;          // 개별 사운드 볼륨(0.0 ~ 1.0)
    FMOD_BOOL m_bool;        // 현재 코드에서는 거의 안 쓰이는 FMOD용 bool
    static std::vector<SoundMgr*> s_instances; // 생성된 SoundMgr 전부 등록
    static float s_masterVolume;               // 전체 마스터 볼륨(0.0 ~ 1.0)

public:
    // 빈 사운드 객체 생성
    SoundMgr();

    // path : 사운드 파일 경로
    // loop : 반복 재생 여부
    SoundMgr(const char* path, bool loop);

    ~SoundMgr();

    // === 전체 마스터 볼륨 제어 ===
    static void SetMasterVolume(float v);                 // 0.0 ~ 1.0
    static float GetMasterVolume();
    static void MasterVolumeUp(float step = SOUND_MASTER_STEP);
    static void MasterVolumeDown(float step = SOUND_MASTER_STEP);

    // FMOD 시스템 초기화 / 해제
    static void Init();
    static void Release();

    // 전체 사운드 제어
    static void StopAll();                 // 모든 채널 정지
    static void PauseAll(bool pause = true);
    static void ResumeAll() { PauseAll(false); }
    static void MuteAll(bool mute = true);

    // 개별 사운드 제어
    void play();
    void pause();
    void resume();
    void stop();

    // 개별 사운드 볼륨 제어
    void volumeUp();
    void volumeDown();
    void volumeSetting(float val);

    // FMOD 내부 업데이트
    void Update(float deltaTime, float Time);

private:
    // 현재 채널에 개별 볼륨 * 마스터 볼륨을 최종 반영
    void ApplyVolume();
};

#endif