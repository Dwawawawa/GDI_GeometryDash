#include "INC_Windows.h"

#include "Collider.h"
#include "RenderHelp.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

// 과제: 해당 코드의 문제는 무엇일까요? 어떻게 개선하면 좋을까요?
// 개선 방향에 대해 서로 토론하고 비교해 보세요.
void GameObject::Update(float deltaTime)
{
    Move(deltaTime);

	if (m_type == ObjectType::PLAYER)
	{
		ApplyGravity(deltaTime);
        Rotate(deltaTime);
	}

    // Collider 업데이트
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    }
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }
	if (m_pColliderTriangle)
	{
		m_pColliderTriangle->apex = m_pos - learning::Vector2f(0, m_pColliderTriangle->height /2);
        m_pColliderTriangle->base = m_pos + learning::Vector2f(0, m_pColliderTriangle->height / 2);
	}

}

void Background::Update(float deltaTime)
{
    // 배경은 업데이트가 필요없음
    // 혹시 배경이 움직일 경우에는 이곳에 구현하면 됨
}

void GameObject::Render(HDC hdc)
{
    DrawBitmap(hdc);
    DrawCollider(hdc);
}


void Background::Render(HDC hdc)
{
    DrawBitmap(hdc);
}

void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}



GameObject::~GameObject()
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }
}

void GameObject::SetColliderCircle(float radius)
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    m_pColliderCircle = new ColliderCircle;

    assert(m_pColliderCircle != nullptr && "Failed to create ColliderCircle!");

    m_pColliderCircle->radius = radius;
    m_pColliderCircle->center = m_pos;
}


void GameObject::SetColliderBox(float width, float height)
{
    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }

    m_pColliderBox = new ColliderBox;

    assert(m_pColliderBox != nullptr && "Failed to create ColliderBox!");

    m_pColliderBox->center = m_pos;
    m_pColliderBox->halfSize.x = width / 2.0f;
    m_pColliderBox->halfSize.y = height / 2.0f;
}

void GameObject::SetColliderTriangle(float width, float height)
{
    if (m_pColliderTriangle)
    {
        delete m_pColliderTriangle;
        m_pColliderTriangle = nullptr;
    }

    m_pColliderTriangle = new ColliderTriangle;

    assert(m_pColliderTriangle != nullptr && "Failed to create ColliderTriangle!");

    m_pColliderTriangle->base = m_pos + learning::Vector2f(0, height / 2);
    m_pColliderTriangle->apex = m_pos - learning::Vector2f(0, height/2);
    m_pColliderTriangle->baseLength = width;
    m_pColliderTriangle->height = height;

}

void GameObject::Rotate(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_rotation += m_rotationSpeed * deltaTime / 1000.0f;

        while (m_rotation >= 360.0f)
        {
            m_rotation -= 360.0f;
        }
    }
    else {
        m_rotation = round(m_rotation / 90.0f) * 90.0f;
    }
}

void GameObject::DrawCollider(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    if (m_pColliderCircle)
    {
        Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
            m_pColliderCircle->center.y - m_pColliderCircle->radius,
            m_pColliderCircle->center.x + m_pColliderCircle->radius,
            m_pColliderCircle->center.y + m_pColliderCircle->radius);
    }

    if (m_pColliderBox)
    {
        Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
            m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
            m_pColliderBox->center.y + m_pColliderBox->halfSize.y);
    }

	if (m_pColliderTriangle)
	{
		// 삼각형 꼭짓점 계산
		learning::Vector2f topPoint = m_pColliderTriangle->apex;
        learning::Vector2f bottomLeft = m_pColliderTriangle->base - learning::Vector2f(m_pColliderTriangle->baseLength / 2, 0);
        learning::Vector2f bottomRight = m_pColliderTriangle->base + learning::Vector2f(m_pColliderTriangle->baseLength / 2, 0);

		// 삼각형 그리기
		POINT points[3] = {
			{(LONG)topPoint.x, (LONG)topPoint.y},
			{(LONG)bottomLeft.x, (LONG)bottomLeft.y},
			{(LONG)bottomRight.x, (LONG)bottomRight.y}
		};
		Polygon(hdc, points, 3);
	}

    // 이전 객체 복원 및 펜 삭제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

void GameObject::Move(float deltaTime)
{
    GameObjectBase::Move(deltaTime);
}

void GameObjectBase::Move(float deltaTime)
{
    m_pos.x += m_dir.x * m_speed * deltaTime;
    m_pos.y += m_dir.y * m_speed * deltaTime;
}

Background::~Background()
{
}

void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;

}

void GameObject::ApplyGravity(float deltaTime)
{
    if (!m_isGrounded) {
        m_verticalVelocity += m_gravityValue * deltaTime;
        m_pos.y += m_verticalVelocity * deltaTime;
    }
}

void GameObject::Jump()
{
    if (m_isGrounded) {
        m_verticalVelocity = -m_jumpForce; // 음수 값이어야 위로 올라감
        m_isGrounded = false;
    }
}


void Background::SetBitmapInfo(BitmapInfo* bitmapInfo)
{
    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");

    m_pBitmapInfo = bitmapInfo;
}

void GameObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

    HDC hBitmapDC = CreateCompatibleDC(hdc); // 1. 메모리와 호환되는...  2. 변수명
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());

    if (m_rotation != 0.0f && m_type == ObjectType::PLAYER) {
        
        // 회전 중심점 계산
		int centerX = m_pos.x;
		int centerY = m_pos.y;
		int halfWidth = m_width / 2;       
		int halfHeight = m_height / 2;

		// 회전 행렬 계산
		float angleRad = m_rotation * 3.14159f / 180.0f;
		float cosTheta = cos(angleRad);
		float sinTheta = sin(angleRad);

		// 회전된 사각형의 꼭지점 계산
		POINT points[3];
		points[0].x = centerX + (-halfWidth * cosTheta - halfHeight * sinTheta);
		points[0].y = centerY + (-halfWidth * sinTheta + halfHeight * cosTheta);
		points[1].x = centerX + (halfWidth * cosTheta - halfHeight * sinTheta);
		points[1].y = centerY + (halfWidth * sinTheta + halfHeight * cosTheta);
		points[2].x = centerX + (-halfWidth * cosTheta + halfHeight * sinTheta);
		points[2].y = centerY + (-halfWidth * sinTheta - halfHeight * cosTheta);

		// 회전된 이미지 그리기
		PlgBlt(hdc, points, hBitmapDC, 0, 0, m_pBitmapInfo->GetWidth(), m_pBitmapInfo->GetHeight(), NULL, 0, 0);
    }
    else{
		// BLENDFUNCTION 설정 (알파 채널 처리)
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
		blend.AlphaFormat = AC_SRC_ALPHA;

		const int x = m_pos.x - m_width / 2;
		const int y = m_pos.y - m_height / 2;

		// 원본 이미지 크기를 사용하도록 수정
		const int srcWidth = m_pBitmapInfo->GetWidth();
		const int srcHeight = m_pBitmapInfo->GetHeight();

		AlphaBlend(hdc, x, y, m_width, m_height, hBitmapDC, 0, 0, srcWidth, srcHeight, blend);
	}

    // 비트맵 핸들 복원
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);
}

void Background::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

    if (m_pBitmapInfo == nullptr) return;
    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
    HDC hBitmapDC = CreateCompatibleDC(hdc); // 1. 메모리와 호환되는...  2. 변수명

    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
    // BLENDFUNCTION 설정 (알파 채널 처리)
    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
    blend.AlphaFormat = AC_SRC_ALPHA;

    int screenWidth = 0;
    int screenHeight = 0;
    learning::GetScreenSize(screenWidth, screenHeight);

    // 소스 이미지 크기를 BitmapInfo에서 가져오기
    int srcWidth = m_pBitmapInfo->GetWidth();
    int srcHeight = m_pBitmapInfo->GetHeight();

    AlphaBlend(hdc, 0, 0, screenWidth, screenHeight,
        hBitmapDC, 0, 0, srcWidth, srcHeight, blend);

    // 비트맵 핸들 복원
    SelectObject(hBitmapDC, hOldBitmap);
    DeleteDC(hBitmapDC);


}


