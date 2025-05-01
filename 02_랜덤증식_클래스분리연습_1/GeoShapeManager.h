#pragma once
#include "INC_Windows.h"
#include "GeoShapeManager.h"

namespace simplegeo
{
	class ShapeBase 
	{
	public:
		virtual void Draw(HDC hdc) = 0;
		virtual ~ShapeBase() {}

		virtual int GetCenterX() { return 0; }
		virtual int GetCenterY() { return 0; }
	};

	class Circle : public ShapeBase
	{
	public:
		Circle(int centerX, int centerY, int radius, COLORREF color);
		void Draw(HDC hdc) override;
		int GetCenterX() override;
		int GetCenterY() override;
	private:
		int m_centerX, m_centerY, m_radius;
		COLORREF m_color;
	};

	class RectangleShape : public ShapeBase
	{
	public:
		RectangleShape(int left, int top, int right, int bottom, COLORREF color);
		void Draw(HDC hdc) override;
		int GetCenterX() override;
		int GetCenterY() override;
	private:
		int m_left, m_top, m_right, m_bottom;
		COLORREF m_color;
	};

	class Line : public ShapeBase
	{
	public:
		Line(int startX, int startY, int endX, int endY, COLORREF color);
		void Draw(HDC hdc) override;
	private:
		int m_startX, m_startY, m_endX, m_endY;
		COLORREF m_color;
	};

	constexpr int MAX_SHAPES = 100; 
	class GeoShapeManager
	{
	public:
		static GeoShapeManager& GetInstance();
		GeoShapeManager(const GeoShapeManager&) = delete;
		GeoShapeManager& operator=(const GeoShapeManager&) = delete;
		GeoShapeManager(GeoShapeManager&&) = delete;
		GeoShapeManager& operator=(GeoShapeManager&&) = delete;
		~GeoShapeManager();

		int CheckShapeArr(int x, int y);
		void AddCircle(int centerX, int centerY, int radius, COLORREF color);
		void AddRectangle(int left, int top, int right, int bottom, COLORREF color);
		void DeleteShape(int index);
		void Draw(HDC hdc);

	private:
		GeoShapeManager();

		ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
		int m_shapeCount = 0;
	};
}


