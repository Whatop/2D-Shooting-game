#include "stdafx.h"
#include "SoundMgr.h"

FMOD_SYSTEM* SoundMgr::g_sound_system;
std::vector<SoundMgr*> SoundMgr::s_instances;
float SoundMgr::s_masterVolume = 1.0f; // 추가
static inline float clamp01(float v) {
    return (v < 0.0f) ? 0.0f : (v > 1.0f ? 1.0f : v);
}


// 생성자들 정리: 멤버 확실히 초기화
SoundMgr::SoundMgr()
    : m_sound(nullptr), m_channel(nullptr), m_volume(SOUND_DEFAULT), m_bool(false) {
    s_instances.push_back(this);
}

SoundMgr::SoundMgr(const char* path, bool loop)
    : m_sound(nullptr), m_channel(nullptr), m_volume(SOUND_DEFAULT), m_bool(false) {
    s_instances.push_back(this);
    if (!g_sound_system) SoundMgr::Init();
    FMOD_MODE mode = FMOD_DEFAULT | (loop ? FMOD_LOOP_NORMAL : 0);
    FMOD_System_CreateSound(g_sound_system, path, mode, nullptr, &m_sound);
}
SoundMgr::~SoundMgr() {
    // 레지스트리에서 제거
    for (auto it = s_instances.begin(); it != s_instances.end(); ++it) {
        if (*it == this) { s_instances.erase(it); break; }
    }
    if (m_sound) { FMOD_Sound_Release(m_sound); m_sound = nullptr; }
}

void SoundMgr::SetMasterVolume(float v) {
    s_masterVolume = clamp01(v);

    // 1) 모든 인스턴스의 채널에 곱적용
    for (auto* inst : s_instances) if (inst) inst->ApplyVolume();

    // 2) FMOD 마스터 채널그룹에도 직접 반영(확실히 들리게)
    if (g_sound_system) {
        FMOD_CHANNELGROUP* cg = nullptr;
        FMOD_System_GetMasterChannelGroup(g_sound_system, &cg);
        if (cg) FMOD_ChannelGroup_SetVolume(cg, s_masterVolume);
    }
}
// 채널에 최종 볼륨 적용
void SoundMgr::ApplyVolume() {
    if (!m_channel) return;
    float effective = clamp01(m_volume * s_masterVolume);
    FMOD_Channel_SetVolume(m_channel, effective);
}

// StopAll 시 내부 채널 포인터도 정리(선택 사항)
void SoundMgr::StopAll() {
    if (!g_sound_system) return;
    FMOD_CHANNELGROUP* master = nullptr;
    FMOD_System_GetMasterChannelGroup(g_sound_system, &master);
    if (master) FMOD_ChannelGroup_Stop(master);
    for (auto* inst : s_instances) if (inst) inst->m_channel = nullptr; // 선택
}


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
void SoundMgr::Init() {
    if (g_sound_system) return; // 중복 방지
    FMOD_System_Create(&g_sound_system);
    FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, nullptr);
}

void SoundMgr::Release() {
    if (!g_sound_system) return;
    FMOD_System_Close(g_sound_system);
    FMOD_System_Release(g_sound_system);
    g_sound_system = nullptr; // 추가
}

void SoundMgr::play() {
    if (!g_sound_system || !m_sound) return;
    FMOD_System_PlaySound(g_sound_system, m_sound, nullptr, false, &m_channel);
    ApplyVolume();
}
// 안전한 널 체크
void SoundMgr::pause() { if (!m_channel) return; FMOD_Channel_SetPaused(m_channel, true); }
void SoundMgr::resume() { if (!m_channel) return; FMOD_Channel_SetPaused(m_channel, false); }
void SoundMgr::stop() { if (!m_channel) return; FMOD_Channel_Stop(m_channel); }

void SoundMgr::volumeUp() { m_volume = clamp01(m_volume + SOUND_WEIGHT); ApplyVolume(); }
void SoundMgr::volumeDown() { m_volume = clamp01(m_volume - SOUND_WEIGHT); ApplyVolume(); }

// SoundMgr.cpp
void SoundMgr::volumeSetting(float value) {
    m_volume = clamp01(value);  // 경계 포함
    ApplyVolume();              // m_volume * s_masterVolume 반영
}

void SoundMgr::Update(float deltaTime, float Time) {
    if (g_sound_system) FMOD_System_Update(g_sound_system);
}

float SoundMgr::GetMasterVolume() { return s_masterVolume; }
void SoundMgr::MasterVolumeUp(float step) { SetMasterVolume(s_masterVolume + step); }
void SoundMgr::MasterVolumeDown(float step) { SetMasterVolume(s_masterVolume - step); }