#include "stdafx.h"
#include "SoundMgr.h"

FMOD_SYSTEM* SoundMgr::g_sound_system;

SoundMgr::SoundMgr()
{
}

SoundMgr::SoundMgr(const char* path, bool loop) {
    if (loop) {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_LOOP_NORMAL, 0, &m_sound);
    }
    else {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_DEFAULT, 0, &m_sound);
    }

    m_channel = nullptr;
    m_volume = SOUND_DEFAULT;
}

SoundMgr::~SoundMgr() {
    FMOD_Sound_Release(m_sound);
}

// === 추가: 전체 제어 구현 ===
void SoundMgr::StopAll()
{
    if (!g_sound_system) return;

    FMOD_CHANNELGROUP* master = nullptr;
    FMOD_System_GetMasterChannelGroup(g_sound_system, &master);
    if (master)
    {
        // 모든 재생 채널(효과음/배경음 포함) 즉시 정지
        FMOD_ChannelGroup_Stop(master);
    }
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
    FMOD_System_Create(&g_sound_system);
    FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, NULL);

}

void SoundMgr::Release() {
    FMOD_System_Close(g_sound_system);
    FMOD_System_Release(g_sound_system);

}


void SoundMgr::play() {
    FMOD_System_PlaySound(g_sound_system, m_sound, NULL, false, &m_channel);

}

void SoundMgr::pause() {
    FMOD_Channel_SetPaused(m_channel, true);

}

void SoundMgr::resume() {
    FMOD_Channel_SetPaused(m_channel, false);

}

void SoundMgr::stop() {
    FMOD_Channel_Stop(m_channel);

}

void SoundMgr::volumeUp() {
    if (m_volume < SOUND_MAX) {
        m_volume += SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

}

void SoundMgr::volumeDown() {
    if (m_volume > SOUND_MIN) {
        m_volume -= SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

}

void SoundMgr::volumeSetting(float valume) {
    if (valume > SOUND_MIN && SOUND_MAX > valume) {
        m_volume = valume;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

}

void SoundMgr::Update(float deltaTime, float Time) {
    FMOD_Channel_IsPlaying(m_channel, &m_bool);

    if (m_bool) {
        FMOD_System_Update(g_sound_system);
    }
}