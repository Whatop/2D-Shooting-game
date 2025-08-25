#pragma once

#ifndef _CSOUND_H_
#define _CSOUND_H_

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f
#define SOUND_MASTER_STEP 0.05f

class SoundMgr :public Singleton<SoundMgr>
{
private:
    static FMOD_SYSTEM* g_sound_system;

    FMOD_SOUND* m_sound;
    FMOD_CHANNEL* m_channel;

    float m_volume;
    FMOD_BOOL m_bool;
    static std::vector<SoundMgr*> s_instances;
    static float s_masterVolume; // 0.0~1.0

public:
    SoundMgr();
    SoundMgr(const char* path, bool loop);
    ~SoundMgr();

    // === master volume controls (global) ===
    static void SetMasterVolume(float v);        // 0.0 ~ 1.0
    static float GetMasterVolume();
    static void MasterVolumeUp(float step = SOUND_MASTER_STEP);
    static void MasterVolumeDown(float step = SOUND_MASTER_STEP);

    static void Init();
    static void Release();
    static void StopAll();                 // 모든 채널 정지
    static void PauseAll(bool pause = true);
    static void ResumeAll() { PauseAll(false); }
    static void MuteAll(bool mute = true);
    void play();
    void pause();
    void resume();
    void stop();
    void volumeUp();
    void volumeDown();
    void volumeSetting(float val);

    void Update(float deltaTime, float Time);


private:

    void ApplyVolume();

};
#endif
