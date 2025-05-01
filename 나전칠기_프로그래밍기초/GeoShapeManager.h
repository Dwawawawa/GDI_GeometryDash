#pragma once
#include "INC_Windows.h"

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
        Circle(int centerX, int centerY, int radius, COLORREF color)
            : m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            // �� ���� �� ����
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            // �귯�ô� ���� ä���� �ʵ��� NULL_BRUSH ���
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            Ellipse(hdc,
                m_centerX - m_radius, m_centerY - m_radius,
                m_centerX + m_radius, m_centerY + m_radius);

            // ���� ��ü ���� �� �� ����
            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);
        }

        int GetCenterX() override
        {
            return m_centerX;
        }

        int GetCenterY() override
        {
            return m_centerY;
        }

    private:
        int m_centerX, m_centerY, m_radius;
        COLORREF m_color;
    };


    class RectangleShape : public ShapeBase
    {
    public:
        RectangleShape(int left, int top, int right, int bottom, COLORREF color)
            : m_left(left), m_top(top), m_right(right), m_bottom(bottom), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

            Rectangle(hdc, m_left, m_top, m_right, m_bottom);

            SelectObject(hdc, hOldPen);
            SelectObject(hdc, hOldBrush);
            DeleteObject(hPen);
        }

        int GetCenterX() override
        {
            return (m_left + m_right)/2;
        }

        int GetCenterY() override
        {
            return (m_top + m_bottom)/2;
        }

    private:
        int m_left, m_top, m_right, m_bottom;
        COLORREF m_color;
    };

    class Line : public ShapeBase
    {
    public:
        Line(int startX, int startY, int endX, int endY, COLORREF color)
            : m_startX(startX), m_startY(startY), m_endX(endX), m_endY(endY), m_color(color) {
        }

        void Draw(HDC hdc) override
        {
            HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

            MoveToEx(hdc, m_startX, m_startY, NULL);
            LineTo(hdc, m_endX, m_endY);

            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        }
    private:
        int m_startX, m_startY, m_endX, m_endY;
        COLORREF m_color;
    };

    constexpr int MAX_SHAPES = 100; // �ִ� ���� ����
    class GeoShapeManager
    {
    public:
        GeoShapeManager() = default;
        ~GeoShapeManager() {

            for (int i = 0; i < MAX_SHAPES; ++i)
            {
                if (m_shapes[i] != nullptr)
                {
                    delete m_shapes[i];
                    m_shapes[i] = nullptr;
                }
            }
        };

        int CheckShapeArr(int x, int y) {
            
            for (int i = 0; i < MAX_SHAPES; i++)
            {
                Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
                RectangleShape* rectangle = dynamic_cast<RectangleShape*>(m_shapes[i]);

                if(circle != nullptr)
                {
                    if (circle->GetCenterX() == x && circle->GetCenterY() == y)
                    {
                        return i;
                    }
                }
                if (rectangle!= nullptr)
                {
                    if (rectangle->GetCenterX() == x && rectangle->GetCenterY() == y)
                    {
                        return i;
                    }
                }

            }
            
            return -1;
        }

        void AddCircle(int centerX, int centerY, int radius, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;

            m_shapes[m_shapeCount] = (ShapeBase*)(new Circle(centerX, centerY, radius, color)); // �Ϲ��� ����ȯ : ���ɽ���
            ++m_shapeCount; 
        }

        void AddRectangle(int left, int top, int right, int bottom, COLORREF color)
        {
            if (m_shapeCount >= MAX_SHAPES) return;

            m_shapes[m_shapeCount] = new RectangleShape(left, top, right, bottom, color);
            ++m_shapeCount;
        }

        //void AddLine(int startX, int startY, int endX, int endY, COLORREF color)
        //{
        //    if (m_shapeCount >= MAX_SHAPES) return;

        //    m_shapes[m_shapeCount] = new Line(startX, startY, endX, endY, color);
        //    ++m_shapeCount;
        //}

        void DeleteShape(int index) {
            delete m_shapes[index];
            m_shapes[index] = nullptr;

            --m_shapeCount;
            for (int i = index; i < m_shapeCount ; i++) {
                m_shapes[i] = m_shapes[i + 1];
            }
        }

        void Draw(HDC hdc)
        {
            for (int i = 0; i < MAX_SHAPES; ++i)
            {
                if (m_shapes[i] == nullptr) break;

                m_shapes[i]->Draw(hdc); // ������ Ÿ�ӿ��� ��ü�� � Ÿ������ �� ���� ����
            }
        }



    private:
        ShapeBase* m_shapes[MAX_SHAPES] = { nullptr, };
        int m_shapeCount = 0;
    };


    extern GeoShapeManager g_GeoShapeManager; // ������ ��.��.�� �� ���� ����(��ü�� �������� ����)
}


