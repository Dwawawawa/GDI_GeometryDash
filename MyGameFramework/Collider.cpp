#include "Collider.h"
#include <iostream>

bool learning::Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs)
{
	return (rhs.center - lhs.center).LengthSquared() <= pow(lhs.radius + rhs.radius, 2);
}

bool learning::Intersect(ColliderBox const& lhs, ColliderBox const& rhs)
{
	// 유효성 검사 추가
	if (lhs.halfSize.x == NULL || lhs.halfSize.y == NULL )
		return false;

	//x-direction
	if (lhs.center.x - lhs.halfSize.x > rhs.center.x + rhs.halfSize.x) return false;
	if (lhs.center.x + lhs.halfSize.x < rhs.center.x - rhs.halfSize.x) return false;

	//y-direction
	if (lhs.center.y - lhs.halfSize.y > rhs.center.y + rhs.halfSize.y) return false;
	if (lhs.center.y + lhs.halfSize.y < rhs.center.y - rhs.halfSize.y) return false;

	return true;

}

bool learning::Intersect(ColliderBox const& box, ColliderTriangle const& triangle)
{
	// 1. 삼각형의 세 점을 구합니다
	Vector2f topPoint = triangle.apex;
	Vector2f bottomLeft = triangle.base - Vector2f(triangle.baseLength / 2, 0);
	Vector2f bottomRight = triangle.base + Vector2f(triangle.baseLength / 2, 0);

	// 2. 삼각형을 포함하는 AABB(Axis-Aligned Bounding Box)를 구합니다
	Vector2f minTriangle = Vector2f(
		std::min({ topPoint.x, bottomLeft.x, bottomRight.x }),
		std::min({ topPoint.y, bottomLeft.y, bottomRight.y })
	);

	Vector2f maxTriangle = Vector2f(
		std::max({ topPoint.x, bottomLeft.x, bottomRight.x }),
		std::max({ topPoint.y, bottomLeft.y, bottomRight.y })
	);

	// 3. 박스와 삼각형의 AABB 간 충돌 검사 (빠른 탈출 검사)
	if (box.center.x + box.halfSize.x < minTriangle.x ||
		box.center.x - box.halfSize.x > maxTriangle.x ||
		box.center.y + box.halfSize.y < minTriangle.y ||
		box.center.y - box.halfSize.y > maxTriangle.y) {
		return false;  // AABB끼리 충돌하지 않으면 삼각형과 박스도 충돌하지 않음
	}

	// 4. 박스의 네 점이 삼각형 내부에 있는지 검사
	Vector2f boxVertices[4] = {
		Vector2f(box.center.x - box.halfSize.x, box.center.y - box.halfSize.y), // 좌하단
		Vector2f(box.center.x + box.halfSize.x, box.center.y - box.halfSize.y), // 우하단
		Vector2f(box.center.x + box.halfSize.x, box.center.y + box.halfSize.y), // 우상단
		Vector2f(box.center.x - box.halfSize.x, box.center.y + box.halfSize.y)  // 좌상단
	};

	for (int i = 0; i < 4; ++i) {
		if (PointInTriangle(boxVertices[i], topPoint, bottomLeft, bottomRight)) {
			return true;  // 박스의 한 점이라도 삼각형 안에 있으면 충돌
		}
	}

	// 5. 삼각형의 각 변과 박스의 변이 교차하는지 검사
	Vector2f boxLines[4][2] = {
		{boxVertices[0], boxVertices[1]}, // 하단
		{boxVertices[1], boxVertices[2]}, // 우측
		{boxVertices[2], boxVertices[3]}, // 상단
		{boxVertices[3], boxVertices[0]}  // 좌측
	};

	Vector2f triangleLines[3][2] = {
		{topPoint, bottomLeft},     // 왼쪽 변
		{topPoint, bottomRight},    // 오른쪽 변
		{bottomLeft, bottomRight}   // 밑변
	};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (LineIntersect(triangleLines[i][0], triangleLines[i][1],
				boxLines[j][0], boxLines[j][1])) {
				return true;  // 선분이 교차하면 충돌
			}
		}
	}

	// 6. 삼각형의 점이 박스 내부에 있는지 검사
	if (PointInBox(topPoint, box) ||
		PointInBox(bottomLeft, box) ||
		PointInBox(bottomRight, box)) {
		return true;
	}

	return false;  // 위의 모든 검사를 통과하면 충돌하지 않음
}

// 점이 삼각형 내부에 있는지 검사하는 함수
bool learning::PointInTriangle(const Vector2f& p, const Vector2f& a, const Vector2f& b, const Vector2f& c)
{
	// 삼각형의 세 점 a, b, c와 점 p에 대해 외적을 이용해 같은 방향인지 검사
	auto sign = [](const Vector2f& p1, const Vector2f& p2, const Vector2f& p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		};

	float d1 = sign(p, a, b);
	float d2 = sign(p, b, c);
	float d3 = sign(p, c, a);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);  // 모든 부호가 같으면 내부에 있음
}

// 점이 박스 내부에 있는지 검사하는 함수
bool learning::PointInBox(const Vector2f& point, const ColliderBox& box)
{
	return (point.x >= box.center.x - box.halfSize.x &&
		point.x <= box.center.x + box.halfSize.x &&
		point.y >= box.center.y - box.halfSize.y &&
		point.y <= box.center.y + box.halfSize.y);
}

// 두 선분이 교차하는지 검사하는 함수
bool learning::LineIntersect(const Vector2f& p1, const Vector2f& p2,
	const Vector2f& p3, const Vector2f& p4)
{
	// 두 선분의 방향 벡터
	float d1 = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
	float d2 = (p2.x - p1.x) * (p4.y - p1.y) - (p4.x - p1.x) * (p2.y - p1.y);
	float d3 = (p4.x - p3.x) * (p1.y - p3.y) - (p1.x - p3.x) * (p4.y - p3.y);
	float d4 = (p4.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p4.y - p3.y);

	// 두 선분이 교차하려면 부호가 다르거나 0이어야 함
	return ((d1 * d2 <= 0) && (d3 * d4 <= 0));
}
