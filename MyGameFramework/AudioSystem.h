#pragma once
#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif
#include <string>
#include <fmod.hpp>
#include <fmod_errors.h>

class AudioSystem
{
public:
	static AudioSystem* GetInstance();
	static void DestroyInstance();

	bool Initialize();
	void Update();
	void Shutdown();

	// 배경 음악 함수
	bool LoadBackgroundMusic(const std::string& filePath);
	void PlayBackgroundMusic(bool loop = true);
	void StopBackgroundMusic();
	void PauseBackgroundMusic();
	void ResumeBackgroundMusic();
	void SetBackgroundMusicVolume(float volume);
	float GetBackgroundMusicVolume();

private:
	AudioSystem();
	~AudioSystem();
	AudioSystem(const AudioSystem&) = delete;
	AudioSystem& operator=(const AudioSystem&) = delete;

	void CheckFMODError(FMOD_RESULT result, const char* message);

	static AudioSystem* s_pInstance;

	FMOD::System* m_pSystem;
	FMOD::Sound* m_pBackgroundMusic;
	FMOD::Channel* m_pBackgroundChannel;
	float m_backgroundVolume;
};
