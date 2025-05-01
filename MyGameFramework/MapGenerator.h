#pragma once
#include "Scene.h"

class PlayScene;
class GameObject;
class MyFirstWndGame;

// 맵 생성을 담당하는 클래스
class MapGenerator
{
public:
	MapGenerator(PlayScene* playScene, MyFirstWndGame* game);
	~MapGenerator() = default;

	// 전체 맵 생성
	void CreateStereoMadnessMap();

private:
	// 맵 섹션별 생성 함수
	void CreateMapSection1();
	void CreateMapSection2();
	void CreateMapSection3();

	// 블록 생성 헬퍼 함수들
	void CreateBlock(float x, float y, int index);
	void CreateSpike(float x, float y, int index);
	void CreateMiniSpike(float x, float y, int index);
	void CreateBlock2(float x, float y, int index);
	void CreatePlatform(float x, float y, int index);
	void CreateSpikeBed(float x, float y, int index);
	void CreateGoal(float x, float y, int index);

	// 참조 변수들
	PlayScene* m_pPlayScene;
	MyFirstWndGame* m_pGame;
};