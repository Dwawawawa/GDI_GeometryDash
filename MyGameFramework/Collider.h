#pragma once
#include "Utillity.h"

namespace learning
{
	struct ColliderCircle
	{
		Vector2f center;
		double radius;
	};

	struct ColliderBox
	{
		Vector2f center;
		Vector2f halfSize;
	};

	struct ColliderTriangle
	{
		Vector2f apex;      // 꼭짓점 (삼각형의 윗부분)
		Vector2f base;      // 밑변의 중심점
		float height;       // 높이
		float baseLength;   // 밑변 길이
	};

	// Circle
	bool Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs);
	// AABB
	bool Intersect(ColliderBox const& lhs, ColliderBox const& rhs);
	// Triangle and Box
	bool Intersect(ColliderBox const& lhs, ColliderTriangle const& rhs);

	// Collider.h에 추가할 내용
	bool PointInTriangle(const Vector2f& p, const Vector2f& a, const Vector2f& b, const Vector2f& c);
	bool PointInBox(const Vector2f& point, const ColliderBox& box);
	bool LineIntersect(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, const Vector2f& p4);
}

