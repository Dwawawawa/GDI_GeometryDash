#pragma once
#include "Scene.h"

class PlayScene;
class GameObject;
class MyFirstWndGame;

// �� ������ ����ϴ� Ŭ����
class MapGenerator
{
public:
	MapGenerator(PlayScene* playScene, MyFirstWndGame* game);
	~MapGenerator() = default;

	// ��ü �� ����
	void CreateStereoMadnessMap();

private:
	// �� ���Ǻ� ���� �Լ�
	void CreateMapSection1();
	void CreateMapSection2();
	void CreateMapSection3();

	// ��� ���� ���� �Լ���
	void CreateBlock(float x, float y, int index);
	void CreateSpike(float x, float y, int index);
	void CreateMiniSpike(float x, float y, int index);
	void CreateBlock2(float x, float y, int index);
	void CreatePlatform(float x, float y, int index);
	void CreateSpikeBed(float x, float y, int index);
	void CreateGoal(float x, float y, int index);

	// ���� ������
	PlayScene* m_pPlayScene;
	MyFirstWndGame* m_pGame;
};