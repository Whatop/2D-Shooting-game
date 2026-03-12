#include "stdafx.h"
#include "SoundMgr.h"

// 정적 멤버 초기화
FMOD_SYSTEM* SoundMgr::g_sound_system;
std::vector<SoundMgr*> SoundMgr::s_instances;
float SoundMgr::s_masterVolume = 1.0f;

// 0.0 ~ 1.0 범위로 강제 제한
static inline float clamp01(float v) {
    return (v < 0.0f) ? 0.0f : (v > 1.0f ? 1.0f : v);
}

// 기본 생성자
// 사운드 파일 없이 빈 객체만 만들고, 기본 볼륨으로 초기화
SoundMgr::SoundMgr()
    : m_sound(nullptr), m_channel(nullptr), m_volume(SOUND_DEFAULT), m_bool(false) {
    s_instances.push_back(this);
}

// 파일 경로를 받아 실제 사운드를 로드하는 생성자
SoundMgr::SoundMgr(const char* path, bool loop)
    : m_sound(nullptr), m_channel(nullptr), m_volume(SOUND_DEFAULT), m_bool(false) {
    s_instances.push_back(this);

    // FMOD 시스템이 아직 없으면 자동 초기화
    if (!g_sound_system) SoundMgr::Init();

    // loop가 true면 반복 재생 모드로 생성
    FMOD_MODE mode = FMOD_DEFAULT | (loop ? FMOD_LOOP_NORMAL : 0);
    FMOD_System_CreateSound(g_sound_system, path, mode, nullptr, &m_sound);
}

// 소멸자
// 등록 리스트에서 자기 자신 제거 후, 로드된 사운드 리소스 해제
SoundMgr::~SoundMgr() {
    for (auto it = s_instances.begin(); it != s_instances.end(); ++it) {
        if (*it == this) {
            s_instances.erase(it);
            break;
        }
    }

    if (m_sound) {
        FMOD_Sound_Release(m_sound);
        m_sound = nullptr;
    }
}

// 전체 마스터 볼륨 설정
void SoundMgr::SetMasterVolume(float v) {
    s_masterVolume = clamp01(v);

    // 이미 존재하는 모든 사운드 인스턴스 채널에 다시 적용
    for (auto* inst : s_instances)
        if (inst) inst->ApplyVolume();

    // FMOD 마스터 채널 그룹에도 직접 반영
    if (g_sound_system) {
        FMOD_CHANNELGROUP* cg = nullptr;
        FMOD_System_GetMasterChannelGroup(g_sound_system, &cg);
        if (cg) FMOD_ChannelGroup_SetVolume(cg, s_masterVolume);
    }
}

// 현재 사운드 채널에 최종 볼륨 적용
// 최종 볼륨 = 개별 볼륨 * 마스터 볼륨
void SoundMgr::ApplyVolume() {
    if (!m_channel) return;

    float effective = clamp01(m_volume * s_masterVolume);
    FMOD_Channel_SetVolume(m_channel, effective);
}

// 모든 사운드 채널 정지
void SoundMgr::StopAll() {
    if (!g_sound_system) return;

    FMOD_CHANNELGROUP* master = nullptr;
    FMOD_System_GetMasterChannelGroup(g_sound_system, &master);
    if (master) FMOD_ChannelGroup_Stop(master);

    // 각 인스턴스의 채널 포인터도 비워서 상태 정리
    for (auto* inst : s_instances)
        if (inst) inst->m_channel = nullptr;
}

// 전체 일시정지 / 해제
void SoundMgr::PauseAll(bool pause)
{
    if (!g_sound_system) return;

    FMOD_CHANNELGROUP* master = nullptr;
    FMOD_System_GetMasterChannelGroup(g_sound_system, &master);
    if (master)
    {
        FMOD_ChannelGroup_SetPaused(master, pause);
    }
}

// 전체 음소거 / 해제
void SoundMgr::MuteAll(bool mute)
{
    if (!g_sound_system) return;

    FMOD_CHANNELGROUP* master = nullptr;
    FMOD_System_GetMasterChannelGroup(g_sound_system, &master);
    if (master)
    {
        FMOD_ChannelGroup_SetMute(master, mute);
    }
}

// FMOD 시스템 초기화
void SoundMgr::Init() {
    if (g_sound_system) return; // 이미 초기화되어 있으면 중복 방지

    FMOD_System_Create(&g_sound_system);
    FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, nullptr);
}

// FMOD 시스템 해제
void SoundMgr::Release() {
    if (!g_sound_system) return;

    FMOD_System_Close(g_sound_system);
    FMOD_System_Release(g_sound_system);
    g_sound_system = nullptr;
}

// 사운드 재생
void SoundMgr::play() {
    if (!g_sound_system || !m_sound) return;

    FMOD_System_PlaySound(g_sound_system, m_sound, nullptr, false, &m_channel);
    ApplyVolume();
}

// 현재 채널 일시정지
void SoundMgr::pause() {
    if (!m_channel) return;
    FMOD_Channel_SetPaused(m_channel, true);
}

// 현재 채널 재개
void SoundMgr::resume() {
    if (!m_channel) return;
    FMOD_Channel_SetPaused(m_channel, false);
}

// 현재 채널 정지
void SoundMgr::stop() {
    if (!m_channel) return;

    FMOD_Channel_Stop(m_channel);
    m_channel = nullptr;
}

// 개별 볼륨 증가
void SoundMgr::volumeUp() {
    m_volume = clamp01(m_volume + SOUND_WEIGHT);
    ApplyVolume();
}

// 개별 볼륨 감소
void SoundMgr::volumeDown() {
    m_volume = clamp01(m_volume - SOUND_WEIGHT);
    ApplyVolume();
}

// 개별 볼륨 직접 설정
void SoundMgr::volumeSetting(float value) {
    m_volume = clamp01(value);
    ApplyVolume();
}

// FMOD 내부 상태 갱신
void SoundMgr::Update(float deltaTime, float Time) {
    if (g_sound_system)
        FMOD_System_Update(g_sound_system);
}

// 현재 마스터 볼륨 반환
float SoundMgr::GetMasterVolume() {
    return s_masterVolume;
}

// 마스터 볼륨 증가
void SoundMgr::MasterVolumeUp(float step) {
    SetMasterVolume(s_masterVolume + step);
}

// 마스터 볼륨 감소
void SoundMgr::MasterVolumeDown(float step) {
    SetMasterVolume(s_masterVolume - step);
}