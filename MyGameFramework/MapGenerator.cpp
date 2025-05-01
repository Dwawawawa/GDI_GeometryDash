#include "MapGenerator.h"
#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <cassert>

using namespace learning;

MapGenerator::MapGenerator(PlayScene* playScene, MyFirstWndGame* game)
	: m_pPlayScene(playScene), m_pGame(game)
{
	assert(m_pPlayScene != nullptr);
	assert(m_pGame != nullptr);
}


void MapGenerator::CreateBlock(float x, float y, int index)
{
	m_pPlayScene->CreateBlock(x, y, index);
}

void MapGenerator::CreateSpike(float x, float y, int index)
{
	m_pPlayScene->CreateSpike(x, y, index);
}

void MapGenerator::CreateMiniSpike(float x, float y, int index)
{
	m_pPlayScene->CreateMiniSpike(x, y, index);
}

void MapGenerator::CreateBlock2(float x, float y, int index)
{
	m_pPlayScene->CreateBlock2(x, y, index);
}

void MapGenerator::CreatePlatform(float x, float y, int index)
{
	m_pPlayScene->CreatePlatform(x, y, index);
}

void MapGenerator::CreateSpikeBed(float x, float y, int index)
{
	m_pPlayScene->CreateSpikeBed(x, y, index);
}

void MapGenerator::CreateGoal(float x, float y, int index)
{
	m_pPlayScene->CreateGoal(x, y, index);
}


void MapGenerator::CreateStereoMadnessMap()
{
	CreateMapSection1();
	CreateMapSection2();
	CreateMapSection3();
	
}

void MapGenerator::CreateMapSection1()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f; // ���� X ��ġ

	// ���� 1: �⺻ Ʃ�丮�� ��Ʈ (������ ��ϰ� ������ũ)

	// �⺻ �ٴ� ��� - ���߿� ��(�׶���)���� ��ü
	for (int i = 0; i < 45; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

	// 1-1
	CreateSpike(baseX + (10 * 122), height - 61.0f - 122.0f, index++);
	
	// 1-2
	CreateMiniSpike(baseX + (19 * 122), height - 30.5f - 122.0f, index++);
	CreateSpike(baseX + (20 * 122), height - 61.0f - 122.0f, index++);

	// 1-3
	CreateSpike(baseX + (29 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (30 * 122), height - 61.0f - 122.0f, index++);
	CreateBlock2(baseX + (31 * 122), height - 61.0f - 122.0f, index++);
	CreateSpikeBed(baseX + (33 * 122), height - 20.0f - 122.0f, index++);
	CreateBlock(baseX + (35 * 122), height - 61.0f - 122.0f, index++);
	CreateBlock2(baseX + (35 * 122), height - 61.0f - 122.0f - 122.0f, index++);
	CreateSpikeBed(baseX + (37 * 122), height - 20.0f - 122.0f, index++);
	CreateBlock(baseX + (39 * 122), height - 61.0f - 122.0f, index++);
	CreateBlock(baseX + (39 * 122), height - 61.0f - 122.0f - 122.0f, index++);
	CreateBlock2(baseX + (39 * 122), height - 61.0f - 122.0f - 122.0f - 122.0f, index++);

	// ��� ī��Ʈ ������Ʈ
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection2()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f + (45 * 122); // ���� 1 ���� ������
	//float baseX = 0.0f;


	// ���� 2: �÷��� ���� ����
	for (int i = 0; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

	// 2-1
	CreateSpike(baseX + (6 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (7 * 122), height - 61.0f - 122.0f, index++);

	// 2-2
	// 2-2�� 2�� ���
	for (int i = 14; i < 27; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
	CreateSpikeBed(baseX + (28 * 122), height - 20.0f - 122.0f, index++);

	// 2-3
	// 2-3�� 2�� ���
	for (int i = 30; i < 45; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
	CreateSpikeBed(baseX + (46 * 122), height - 20.0f - 122.0f, index++);
	CreateSpike(baseX + (37 * 122), height - 61.0f - 122.0f * 2, index++);

	// 2-4
	// 2-4�� 2,3�� ���
	for (int i = 48; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - 122.0f, index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f - 122.0f, index++);
	}
	CreateSpike(baseX + (55 * 122), height - 61.0f - 122.0f * 3, index++);

	// ��� ī��Ʈ ������Ʈ
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection3()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f + (45 * 122) + (63 * 122); // ���� 2 ���� ������
	//float baseX = 0.0f + (63 * 122);
	//float baseX = 0.0f;

	// ���� 3: ���� ����

	// 3-1 ����
	for (int i = 0; i < 8; i++) {
		CreateSpikeBed(baseX + 122 + (i * 122 * 3), height - 20.0f, index++);
	}
	for (int i = 0; i < 5; i++) {
		CreatePlatform(baseX + 122 * 3 +(i * 122* 4), height - 61.0f - (122.0f * 3) - (122.0f * i), index++);
	}

	// 3-2
	//��������
	for (int i = 21; i < 24; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}
	for (int i = 24; i < 45; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}

	// 3-3 (���� 4���� �� 2��) * 2��
	for (int i = 27; i < 31; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}
	for (int i = 28; i < 30; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
	}
	for (int i = 37; i < 41; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}
	for (int i = 38; i < 40; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
	}

	// 3-4 �� �� �������� �ö󰬴� ���� �������� �������� 
	for (int i = 45; i < 55; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	for (int i = 48; i < 52; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}
	for (int i = 48; i < 52; i++) {
		CreateSpike(baseX + (i * 122), height - 29.5f - (122.0f * 7), index++);
	}

	for (int i = 55; i < 61; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}

	{
		int i = 60;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}

	{
		int i = 61;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}
	for (int i = 61; i < 70; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	// 3-5 �������� �� ������ ����
	// ��������
	for (int i = 70; i < 73; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	for (int i = 0; i < 16; i++) {//���� ����
		CreateSpikeBed(baseX + (70 * 122) + (i * 122 * 3), height - 20.0f, index++);
	}

	for (int i = 75; i < 79; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	{
		int i = 78;
		CreateSpike(baseX + (i * 122), height - 29.5f - (122.0f * 4), index++);
	}


	for (int i = 80; i < 87; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}
	{
		int i = 86;
		CreateSpike(baseX + (i * 122), height - 29.5f - (122.0f * 3), index++);
	}

	for (int i = 88; i < 93; i++) {
		CreatePlatform(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}

	// �ö󰡱�
	for (int i = 0; i < 6; i++) { // 6��° ���� ����
		//���� 96
		CreatePlatform(baseX + (96 * 122) + (i * 122 * 4), height - 61.0f - (122.0f * 2) - (122.0f * i), index++);
	}

	{
		int i = 116;
		CreateSpike(baseX + (i * 122), height - 29.5f - (122.0f * 8), index++);
	}

	// 1�������� ����
	for (int i = 114; i < 118; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	for (int i = 118; i < 141; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}


	// ����
	for (int i = 118; i < 124; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 9), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 8), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
	}
	
	for (int i = 124; i < 135; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 9), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 8), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}

	for (int i = 135; i < 141; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 9), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 8), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
	}


	{
		int i = 138;
		CreateGoal(baseX + (i * 122), height - 91.5f - (122.0f * 4), index++);
	}



	// ��� ī��Ʈ ������Ʈ
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}
