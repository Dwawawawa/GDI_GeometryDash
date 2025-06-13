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
	//스테이지 1
	CreateMapSection1();
	CreateMapSection2();
	CreateMapSection3();

	//스테이지 2
	CreateMapSection4();
	CreateMapSection5();

}

void MapGenerator::CreateMapSection1()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f; // 시작 X 위치

	// 섹션 1: 기본 튜토리얼 파트 (간단한 블록과 스파이크)

	for (int i = 0; i < 45; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

	// 1-1
	CreateSpike(baseX + (10 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (11 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (12 * 122), height - 61.0f - 122.0f, index++);

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

	// 블록 카운트 업데이트
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection2()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f + (45 * 122); // 섹션 1 이후 시작점



	// 섹션 2: 플랫폼 점프 섹션
	for (int i = 0; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

	// 2-1
	CreateSpike(baseX + (6 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (7 * 122), height - 61.0f - 122.0f, index++);

	// 2-2
	// 2-2의 2층 블록
	for (int i = 14; i < 27; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
	CreateSpikeBed(baseX + (28 * 122), height - 20.0f - 122.0f, index++);

	// 2-3
	// 2-3의 2층 블록
	for (int i = 30; i < 45; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
	CreateSpikeBed(baseX + (46 * 122), height - 20.0f - 122.0f, index++);
	CreateSpike(baseX + (37 * 122), height - 61.0f - 122.0f * 2, index++);

	// 2-4
	// 2-4의 2,3층 블록
	for (int i = 48; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - 122.0f, index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f - 122.0f, index++);
	}
	CreateSpike(baseX + (55 * 122), height - 61.0f - 122.0f * 3, index++);

	// 블록 카운트 업데이트
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection3()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f + (45 * 122) + (63 * 122); // 섹션 2 이후 시작점

	// 섹션 3: 점프 조합

	// 3-1 점프
	for (int i = 0; i < 8; i++) {
		CreateSpikeBed(baseX + 122 + (i * 122 * 3), height - 20.0f, index++);
	}
	for (int i = 0; i < 5; i++) {
		CreatePlatform(baseX + 122 * 3 + (i * 122 * 4), height - 61.0f - (122.0f * 3) - (122.0f * i), index++);
	}

	// 3-2
	//낭떠러지
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

	// 3-3 (세모 4개와 바 2개) * 2개
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

	// 3-4 한 층 내려갔다 올라갔다 두층 내려가는 낭떠러지 
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

	// 3-5 낭떠러지 끝 점프맵 시작
	// 내려가기
	for (int i = 70; i < 73; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	for (int i = 0; i < 16; i++) {//여기 수정
		CreateSpikeBed(baseX + (71 * 122) + (i * 122 * 3), height - 20.0f, index++);
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

	// 올라가기
	for (int i = 0; i < 6; i++) { // 6번째 가시 있음
		//시작 96
		CreatePlatform(baseX + (96 * 122) + (i * 122 * 4), height - 61.0f - (122.0f * 2) - (122.0f * i), index++);
	}

	{
		int i = 116;
		CreateSpike(baseX + (i * 122), height - 29.5f - (122.0f * 8), index++);
	}

	// 1스테이지 종료
	for (int i = 114; i < 118; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	for (int i = 118; i < 141; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}


	// 위에
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



	// 블록 카운트 업데이트
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection4()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	//float baseX = 0.0f; // 시작 X 위치
	float baseX = 0.0f + (45 * 122) + (63 * 122) + (140 * 122); // 섹션 4의 시작점

	//4-1
	//평지
	for (int i = 0; i < 10; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}

	{
		int i = 11;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥1
	{
		int i = 13;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}

	{
		int i = 15;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥2
	{
		int i = 17;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}


	{
		int i = 19;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥3 툭
	
	{
		int i = 20;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}

	//기둥3
	{
		int i = 21;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	{
		int i = 23;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}


	//기둥4 툭

	{
		int i = 24;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	//기둥4
	{
		int i = 25;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		//CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);

		CreateGoal(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);

		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}

	// 다시 평지
	for (int i = 26; i < 36; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}
	{
		int i = 37;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥1
	{
		int i = 39;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}

	{
		int i = 41;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥2
	{
		int i = 43;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}


	{
		int i = 45;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥3
	{
		int i = 47;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	{
		int i = 49;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥4
	{
		int i = 51;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}

	{
		int i = 53;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥5
	{
		int i = 55;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}
	{
		int i = 57;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥6
	{
		int i = 59;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
	}
	{
		int i = 61;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	//기둥7
	{
		int i = 63;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 8), index++);
	}
	// 내려가는 플랫폼
	{
		int i = 64;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 8), index++);
	}
	for(int i = 66; i < 68; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 7), index++);
	}
	for (int i = 69; i < 71; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 6), index++);
	}
	for (int i = 72; i < 75; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 5), index++);
	}

	for (int i = 78; i < 80; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 4), index++);
	}

	// 평지 필요하지 않을까?
	for (int i = 81; i < 84; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 28.0f - (122.0f * 4), index++);
	}
	// 평지 필요하지 않을까?
	for (int i = 87; i < 91; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 28.0f - (122.0f * 3), index++);
	}

	{
		int i = 92;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 92;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}
	{
		int i = 95;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 94;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 96;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	
	// 0층에서 놈
	for (int i = 97; i < 156; i++) {
		CreateBlock(baseX + (i * 122), height - 62.0f/*61로 바꿔라*/ - (122.0f * -1), index++);
	}
	

	for (int i = 102; i < 104; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	{
		int i = 104;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}

	{
		int i = 106;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	for (int i = 107; i < 109; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 109;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}


	for (int i = 110; i < 113; i++)
	{
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}


	for (int i = 115; i < 117; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 114;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 117;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 120;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	for (int i = 119; i < 121; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}
	for (int i = 121; i < 124; i++)
	{
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	for (int i = 124; i < 126; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}

	for (int i = 132; i < 134; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	for (int i = 140; i < 143; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	{
		int i = 149;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	{
		int i = 156;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}



	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());
}

void MapGenerator::CreateMapSection5()
{
	int height = m_pGame->GetHeight();
	int index = m_pPlayScene->GetBlockStartIndex() + m_pPlayScene->GetBlockCount();
	float baseX = 0.0f + (45 * 122) + (63 * 122) + (140 * 122) + (150* 122);
	//float baseX = 0.0f;
	

	// 0층에서 놈
	for (int i = 0; i < 130; i++) {
		CreateBlock(baseX + (i * 122), height - 62.0f/*61로 바꿔라*/ - (122.0f * -1), index++);
	}

	// 다시 올라간다. 
	for (int i = 7; i < 9; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	{
		int i = 9;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	{
		int i = 11;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 13;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}
	{
		int i = 15;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 17;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}
	{
		int i = 19;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 21;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}
	{
		int i = 23;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}
	{
		int i = 26;
		CreateSpikeBed(baseX + (i * 122), height - 20.0f, index++);
	}

	{
		int i = 22;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 3), index++);
	}
	{
		int i = 24;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}
	{
		int i = 26;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 28;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}

	for (int i = 34; i < 36; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}

	for (int i = 42; i < 46; i++) {
	
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}

	// 세모네모
	{
		int i = 50;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	{
		int i = 51;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}


	//가시밭
	for (int i = 0; i < 17; i++)  
	{
		CreateSpikeBed(baseX + (59 * 122) + (i * 3 * 122), height - 20.0f - (122.0f * 0), index++);
	}
	for (int i = 58; i < 62; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 66;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 70;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}
	{
		int i = 74;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 3), index++);
	}
	for (int i = 78; i < 81; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 4), index++);
	}


	//2개
	{
		int i = 82;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 6), index++);
	}

	{
		int i = 84;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 5), index++);
	}
	{
		int i = 86;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}

	{
		int i = 88;
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock2(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
		CreateMiniSpike(baseX + (i * 122), height - 30.5f - (122.0f * 4), index++);

		CreateGoal(baseX + (i * 122), height - 80.0f - (122.0f * 5), index++);
	}


	{
		int i = 92;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 94;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}
	{
		int i = 96;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 97;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}

	for (int i = 99; i < 102; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}
	{
		int i = 104;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	for (int i = 106; i < 108; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	for (int i = 109; i < 111; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}



	// 3개!
	for (int i = 117; i < 120; i++) {
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 4), index++);
	}



	//마지막
	for (int i = 0; i < 18; i++)  
	{
		CreateSpikeBed(baseX + (130 * 122) + (i * 3 * 122), height - 20.0f - (122.0f * 0), index++);
	}
	{
		int i = 129;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}

	{
		int i = 130;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 131;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}

	{
		int i = 135;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}

	{
		int i = 139;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 140;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 141;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
	}
	{
		int i = 142;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
	}
	{
		int i = 143;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 144;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}

	{
		int i = 148;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}

	{
		int i = 150;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}

	{
		int i = 152;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 153;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}
	{
		int i = 154;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 0), index++);
	}

	{
		int i = 158;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 160;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}
	for(int i = 162; i < 166; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}
	{
		int i = 168;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);
	}

	{
		int i = 170;
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 1), index++);
	}

	for (int i = 174; i < 178; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}

	{
		int i = 177;
		CreateSpike(baseX + (i * 122), height - 61.0f - (122.0f * 3), index++);
	}

	for (int i = 179; i < 182; i++)
	{
		CreatePlatform(baseX + (i * 122), height - 94.0f - (122.0f * 2), index++);
	}


	{
		int i = 183;
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 0), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 1), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 2), index++);


		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 9), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 7), index++);
		CreateBlock(baseX + (i * 122), height - 61.0f - (122.0f * 8), index++);
	}
	
	for (int i = 183; i < 200; i++) {
		CreateBlock(baseX + (i * 122), height - 62.0f/*61로 바꿔라*/ - (122.0f * -1), index++);
	}
	m_pPlayScene->UpdateBlockCount(index - m_pPlayScene->GetBlockStartIndex());

}
