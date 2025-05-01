#include "GameManager.h"
#include "GeoShapeManager.h"
#include <random>


namespace game
{

    int GameManager::MakeRandomNum() {

        int seed = rand();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(0, 99);

        int num = dist(gen);

        return num;
    }

    void GameManager::MultiplyShape(const int& width, const int& height, simplegeo::GeoShapeManager& g_GeoShapeManager)
    {
        static const int circleRadius = 20;
        static const int rectangleEdge = 20;

        if (MakeRandomNum() % 2 == 0)
        {
            g_GeoShapeManager.AddCircle(width * MakeRandomNum() / 100, height * MakeRandomNum() / 100, circleRadius, RGB(255, 0, 0));
        }
        else {
            int rectangleX = width * MakeRandomNum() / 100;
            int rectangleY = height * MakeRandomNum() / 100;
            g_GeoShapeManager.AddRectangle(rectangleX - rectangleEdge, rectangleY + rectangleEdge, rectangleX + rectangleEdge, rectangleY - rectangleEdge, RGB(0, 0, 255));
        }
    }

}