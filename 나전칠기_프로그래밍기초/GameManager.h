#pragma once
#include "GeoShapeManager.h"
#include "WindowManager.h"

class GameManager {
public:
	GameManager();
	int MakeRandomNum();
	void MultiplyRandomShape();
	
private:
	static const int circleRadius = 20;
	static const int rectangleEdge = 20;

};