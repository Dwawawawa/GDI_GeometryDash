#include "Collider.h"
#include <iostream>

bool learning::Intersect(ColliderCircle const& lhs, ColliderCircle const& rhs)
{
	return (rhs.center - lhs.center).LengthSquared() <= pow(lhs.radius + rhs.radius, 2);
}

bool learning::Intersect(ColliderBox const& lhs, ColliderBox const& rhs)
{
	// ��ȿ�� �˻� �߰�
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
	// 1. �ﰢ���� �� ���� ���մϴ�
	Vector2f topPoint = triangle.apex;
	Vector2f bottomLeft = triangle.base - Vector2f(triangle.baseLength / 2, 0);
	Vector2f bottomRight = triangle.base + Vector2f(triangle.baseLength / 2, 0);

	// 2. �ﰢ���� �����ϴ� AABB(Axis-Aligned Bounding Box)�� ���մϴ�
	Vector2f minTriangle = Vector2f(
		std::min({ topPoint.x, bottomLeft.x, bottomRight.x }),
		std::min({ topPoint.y, bottomLeft.y, bottomRight.y })
	);

	Vector2f maxTriangle = Vector2f(
		std::max({ topPoint.x, bottomLeft.x, bottomRight.x }),
		std::max({ topPoint.y, bottomLeft.y, bottomRight.y })
	);

	// 3. �ڽ��� �ﰢ���� AABB �� �浹 �˻� (���� Ż�� �˻�)
	if (box.center.x + box.halfSize.x < minTriangle.x ||
		box.center.x - box.halfSize.x > maxTriangle.x ||
		box.center.y + box.halfSize.y < minTriangle.y ||
		box.center.y - box.halfSize.y > maxTriangle.y) {
		return false;  // AABB���� �浹���� ������ �ﰢ���� �ڽ��� �浹���� ����
	}

	// 4. �ڽ��� �� ���� �ﰢ�� ���ο� �ִ��� �˻�
	Vector2f boxVertices[4] = {
		Vector2f(box.center.x - box.halfSize.x, box.center.y - box.halfSize.y), // ���ϴ�
		Vector2f(box.center.x + box.halfSize.x, box.center.y - box.halfSize.y), // ���ϴ�
		Vector2f(box.center.x + box.halfSize.x, box.center.y + box.halfSize.y), // ����
		Vector2f(box.center.x - box.halfSize.x, box.center.y + box.halfSize.y)  // �»��
	};

	for (int i = 0; i < 4; ++i) {
		if (PointInTriangle(boxVertices[i], topPoint, bottomLeft, bottomRight)) {
			return true;  // �ڽ��� �� ���̶� �ﰢ�� �ȿ� ������ �浹
		}
	}

	// 5. �ﰢ���� �� ���� �ڽ��� ���� �����ϴ��� �˻�
	Vector2f boxLines[4][2] = {
		{boxVertices[0], boxVertices[1]}, // �ϴ�
		{boxVertices[1], boxVertices[2]}, // ����
		{boxVertices[2], boxVertices[3]}, // ���
		{boxVertices[3], boxVertices[0]}  // ����
	};

	Vector2f triangleLines[3][2] = {
		{topPoint, bottomLeft},     // ���� ��
		{topPoint, bottomRight},    // ������ ��
		{bottomLeft, bottomRight}   // �غ�
	};

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (LineIntersect(triangleLines[i][0], triangleLines[i][1],
				boxLines[j][0], boxLines[j][1])) {
				return true;  // ������ �����ϸ� �浹
			}
		}
	}

	// 6. �ﰢ���� ���� �ڽ� ���ο� �ִ��� �˻�
	if (PointInBox(topPoint, box) ||
		PointInBox(bottomLeft, box) ||
		PointInBox(bottomRight, box)) {
		return true;
	}

	return false;  // ���� ��� �˻縦 ����ϸ� �浹���� ����
}

// ���� �ﰢ�� ���ο� �ִ��� �˻��ϴ� �Լ�
bool learning::PointInTriangle(const Vector2f& p, const Vector2f& a, const Vector2f& b, const Vector2f& c)
{
	// �ﰢ���� �� �� a, b, c�� �� p�� ���� ������ �̿��� ���� �������� �˻�
	auto sign = [](const Vector2f& p1, const Vector2f& p2, const Vector2f& p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
		};

	float d1 = sign(p, a, b);
	float d2 = sign(p, b, c);
	float d3 = sign(p, c, a);

	bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);  // ��� ��ȣ�� ������ ���ο� ����
}

// ���� �ڽ� ���ο� �ִ��� �˻��ϴ� �Լ�
bool learning::PointInBox(const Vector2f& point, const ColliderBox& box)
{
	return (point.x >= box.center.x - box.halfSize.x &&
		point.x <= box.center.x + box.halfSize.x &&
		point.y >= box.center.y - box.halfSize.y &&
		point.y <= box.center.y + box.halfSize.y);
}

// �� ������ �����ϴ��� �˻��ϴ� �Լ�
bool learning::LineIntersect(const Vector2f& p1, const Vector2f& p2,
	const Vector2f& p3, const Vector2f& p4)
{
	// �� ������ ���� ����
	float d1 = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
	float d2 = (p2.x - p1.x) * (p4.y - p1.y) - (p4.x - p1.x) * (p2.y - p1.y);
	float d3 = (p4.x - p3.x) * (p1.y - p3.y) - (p1.x - p3.x) * (p4.y - p3.y);
	float d4 = (p4.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p4.y - p3.y);

	// �� ������ �����Ϸ��� ��ȣ�� �ٸ��ų� 0�̾�� ��
	return ((d1 * d2 <= 0) && (d3 * d4 <= 0));
}
