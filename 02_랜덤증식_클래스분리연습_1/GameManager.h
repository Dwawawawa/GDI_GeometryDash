#pragma once  
#include "GameManager.h"  
#include "GeoShapeManager.h"  

namespace game  
{  
	class GameManager
	{
	public:
		int MakeRandomNum();
		void MultiplyShape(const int& width, const  int& height, simplegeo::GeoShapeManager& manager);
	private:
		
	};
}