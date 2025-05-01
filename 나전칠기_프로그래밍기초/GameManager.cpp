#include "GameManager.h"
#include <random>

GameManager::GameManager()
{
}

int GameManager::MakeRandomNum() {

    int seed = rand();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 99);

    int num = dist(gen);

    return num;
}
void GameManager::MultiplyRandomShape()
{
    WindowManager* a = new WindowManager;
    int width = a->GetClientWidth();
    int height = a->GetClientHeight();

    if (MakeRandomNum() % 2 == 0)
    {
        simplegeo::g_GeoShapeManager.AddCircle(width * MakeRandomNum() / 100, height * MakeRandomNum() / 100, circleRadius, RGB(255, 0, 0));
    }
    else {
        int rectangleX = width * MakeRandomNum() / 100;
        int rectangleY = height * MakeRandomNum() / 100;
        simplegeo::g_GeoShapeManager.AddRectangle(rectangleX - rectangleEdge, rectangleY + rectangleEdge, rectangleX + rectangleEdge, rectangleY - rectangleEdge, RGB(0, 0, 255));
    }
}


