#include "GeoShapeManager.h"

namespace simplegeo
{
   	Circle::Circle(int centerX, int centerY, int radius, COLORREF color): m_centerX(centerX), m_centerY(centerY), m_radius(radius), m_color(color) {}

    void Circle::Draw(HDC hdc)
    {
        // 펜 생성 및 선택
        HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        // 브러시는 내부 채우지 않도록 NULL_BRUSH 사용
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        Ellipse(hdc,
            m_centerX - m_radius, m_centerY - m_radius,
            m_centerX + m_radius, m_centerY + m_radius);

        // 이전 객체 복원 및 펜 삭제
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }

    int Circle::GetCenterX()
    {
        return m_centerX;
    }

    int Circle::GetCenterY()
    {
        return m_centerY;
    }


    RectangleShape::RectangleShape(int left, int top, int right, int bottom, COLORREF color)
        : m_left(left), m_top(top), m_right(right), m_bottom(bottom), m_color(color) {
    }

    void RectangleShape::Draw(HDC hdc)
    {
        HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        Rectangle(hdc, m_left, m_top, m_right, m_bottom);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }

    int RectangleShape::GetCenterX()
    {
        return (m_left + m_right) / 2;
    }

    int RectangleShape::GetCenterY()
    {
        return (m_top + m_bottom) / 2;
    }


    Line::Line(int startX, int startY, int endX, int endY, COLORREF color): m_startX(startX), m_startY(startY), m_endX(endX), m_endY(endY), m_color(color) {}

    void Line::Draw(HDC hdc)
    {
        HPEN hPen = CreatePen(PS_SOLID, 2, m_color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        MoveToEx(hdc, m_startX, m_startY, NULL);
        LineTo(hdc, m_endX, m_endY);

        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }


    GeoShapeManager& GeoShapeManager::GetInstance() 
    {
        static GeoShapeManager instance;
        return instance;
    };

    GeoShapeManager::GeoShapeManager() = default;
 
    GeoShapeManager::~GeoShapeManager() 
    {
        for (int i = 0; i < MAX_SHAPES; ++i)
        {
            if (m_shapes[i] != nullptr)
            {
                delete m_shapes[i];
                m_shapes[i] = nullptr;
            }
        }
    };

    int GeoShapeManager::CheckShapeArr(int x, int y) {

        for (int i = 0; i < MAX_SHAPES; i++)
        {
            Circle* circle = dynamic_cast<Circle*>(m_shapes[i]);
            RectangleShape* rectangle = dynamic_cast<RectangleShape*>(m_shapes[i]);

            if (circle != nullptr)
            {
                if (circle->GetCenterX() == x && circle->GetCenterY() == y)
                {
                    return i;
                }
            }
            if (rectangle != nullptr)
            {
                if (rectangle->GetCenterX() == x && rectangle->GetCenterY() == y)
                {
                    return i;
                }
            }

        }

        return -1;
    }

    void GeoShapeManager::AddCircle(int centerX, int centerY, int radius, COLORREF color)
    {
        if (m_shapeCount >= MAX_SHAPES) return;

        m_shapes[m_shapeCount] = (ShapeBase*)(new Circle(centerX, centerY, radius, color)); // 암묵적 형변환 : 업케스팅
        ++m_shapeCount;
    }

    void GeoShapeManager::AddRectangle(int left, int top, int right, int bottom, COLORREF color)
    {
        if (m_shapeCount >= MAX_SHAPES) return;

        m_shapes[m_shapeCount] = new RectangleShape(left, top, right, bottom, color);
        ++m_shapeCount;
    }


    void GeoShapeManager::DeleteShape(int index) {
        delete m_shapes[index];
        m_shapes[index] = nullptr;

        --m_shapeCount;
        for (int i = index; i < m_shapeCount; i++) {
            m_shapes[i] = m_shapes[i + 1];
        }
    }

    void GeoShapeManager::Draw(HDC hdc)
    {
        for (int i = 0; i < MAX_SHAPES; ++i)
        {
            if (m_shapes[i] == nullptr) break;

            m_shapes[i]->Draw(hdc); // 컴파일 타임에는 객체가 어떤 타입인지 알 수가 없음
        }
    }


}
