#include "AudioSystem.h"
#include <iostream>

AudioSystem* AudioSystem::s_pInstance = nullptr;

AudioSystem* AudioSystem::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new AudioSystem();
	}
	return s_pInstance;
}

void AudioSystem::DestroyInstance()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}

AudioSystem::AudioSystem()
	: m_pSystem(nullptr)
	, m_pBackgroundMusic(nullptr)
	, m_pBackgroundChannel(nullptr)
	, m_backgroundVolume(1.0f)
{
}

AudioSystem::~AudioSystem()
{
	Shutdown();
}

bool AudioSystem::Initialize()
{
	FMOD_RESULT result;

	// FMOD 시스템 생성
	result = FMOD::System_Create(&m_pSystem);
	if (result != FMOD_OK)
	{
		CheckFMODError(result, "Failed to create FMOD system");
		return false;
	}

	// FMOD 시스템 초기화
	result = m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK)
	{
		CheckFMODError(result, "Failed to initialize FMOD system");
		return false;
	}

	std::cout << "FMOD system initialized successfully!" << std::endl;
	return true;
}

void AudioSystem::Update()
{
	if (m_pSystem)
	{
		m_pSystem->update();
	}
}

void AudioSystem::Shutdown()
{
	if (m_pBackgroundMusic)
	{
		m_pBackgroundMusic->release();
		m_pBackgroundMusic = nullptr;
	}

	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
		m_pSystem = nullptr;
	}
}

bool AudioSystem::LoadBackgroundMusic(const std::string& filePath)
{
	if (!m_pSystem)
	{
		std::cerr << "FMOD system not initialized!" << std::endl;
		return false;
	}

	// 이미 로드된 배경 음악이 있다면 해제
	if (m_pBackgroundMusic)
	{
		m_pBackgroundMusic->release();
		m_pBackgroundMusic = nullptr;
	}

	// 새 배경 음악 로드
	FMOD_RESULT result = m_pSystem->createSound(
		filePath.c_str(),
		FMOD_DEFAULT,
		nullptr,
		&m_pBackgroundMusic
	);

	if (result != FMOD_OK)
	{
		CheckFMODError(result, "Failed to load background music");
		return false;
	}

	std::cout << "Background music loaded: " << filePath << std::endl;
	return true;
}

void AudioSystem::PlayBackgroundMusic(bool loop)
{
	if (!m_pSystem || !m_pBackgroundMusic)
	{
		std::cerr << "FMOD system or background music not initialized!" << std::endl;
		return;
	}

	// 루프 설정
	if (loop)
	{
		m_pBackgroundMusic->setMode(FMOD_LOOP_NORMAL);
	}
	else
	{
		m_pBackgroundMusic->setMode(FMOD_LOOP_OFF);
	}

	// 음악 재생
	FMOD_RESULT result = m_pSystem->playSound(
		m_pBackgroundMusic,
		nullptr,
		false,
		&m_pBackgroundChannel
	);

	if (result != FMOD_OK)
	{
		CheckFMODError(result, "Failed to play background music");
		return;
	}

	// 볼륨 설정
	m_pBackgroundChannel->setVolume(m_backgroundVolume);
}

void AudioSystem::StopBackgroundMusic()
{
	if (m_pBackgroundChannel)
	{
		m_pBackgroundChannel->stop();
	}
}

void AudioSystem::PauseBackgroundMusic()
{
	if (m_pBackgroundChannel)
	{
		m_pBackgroundChannel->setPaused(true);
	}
}

void AudioSystem::ResumeBackgroundMusic()
{
	if (m_pBackgroundChannel)
	{
		m_pBackgroundChannel->setPaused(false);
	}
}

void AudioSystem::SetBackgroundMusicVolume(float volume)
{
	m_backgroundVolume = volume;
	if (m_pBackgroundChannel)
	{
		m_pBackgroundChannel->setVolume(volume);
	}
}

float AudioSystem::GetBackgroundMusicVolume()
{
	return m_backgroundVolume;
}

void AudioSystem::CheckFMODError(FMOD_RESULT result, const char* message)
{
	if (result != FMOD_OK)
	{
		std::cerr << message << ": " << FMOD_ErrorString(result) << std::endl;
	}
}