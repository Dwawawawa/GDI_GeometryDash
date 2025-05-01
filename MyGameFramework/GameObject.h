#pragma once

#include "Utillity.h"
#include "Collider.h" 
#include <algorithm>
#include <windows.h>
// [CHECK]. namespace 포함해서 전방 선언해야 함
namespace learning
{
    struct ColliderCircle;
    struct ColliderBox;
    struct ColliderTriangle;
}

namespace renderHelp
{
    class BitmapInfo;
}

enum class ObjectType
{
    PLAYER, 
    BLOCK1,
    BLOCK2,
    BACKGROUND,
    SPIKE,          
    MINISPKIE,  
    PLATFORM, 
    SPIKEBED,       
    GOAL            
};

constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
    using Vector2f = learning::Vector2f;
public:
    GameObjectBase() = delete;
    GameObjectBase(const GameObjectBase&) = delete;

    GameObjectBase(ObjectType type) : m_type(type) {}

    virtual ~GameObjectBase() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hdc) = 0;

    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);

    void SetWidth(int width) { m_width = width; }
    void SetHeight(int height) { m_height = height; }

    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }


    

protected:

    void Move(float deltaTime);

protected:
    ObjectType m_type;

    Vector2f m_pos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)

    float m_speed = 0.0f; // 속력
    int m_width = 0;
    int m_height = 0;

    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
    using ColliderTriangle = learning::ColliderTriangle;

    using BitmapInfo = renderHelp::BitmapInfo;

public:
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc) override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);
    void SetColliderTriangle(float width, float height);

    void SetBitmapInfo(BitmapInfo* bitmapInfo);
    
	// 중력과 점프
	void ApplyGravity(float deltaTime);
	void Jump();
	bool IsGrounded() const { return m_isGrounded; }
	void SetGrounded(bool isGrounded) { m_isGrounded = isGrounded; }

	const ColliderBox* GetColliderBox() const { return m_pColliderBox; }
    const ColliderTriangle* GetColliderTriangle() const { return m_pColliderTriangle; }
	float GetVerticalVelocity() const { return m_verticalVelocity; }

    // 회전
	void SetRotation(float rotation) { m_rotation = rotation; }
	float GetRotation() const { return m_rotation; }
    void Rotate(float deltaTime);

protected:

    void DrawCollider(HDC hdc);
    void DrawBitmap(HDC hdc);

    void Move(float deltaTime);

    // collides
    ColliderCircle* m_pColliderCircle = nullptr;
    ColliderBox* m_pColliderBox = nullptr;
    ColliderTriangle* m_pColliderTriangle = nullptr;

    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;

	// 중력과 점프
	bool m_isGrounded = false;
	float m_verticalVelocity = 0.0f;
	float m_gravityValue = 0.008f; 
	float m_jumpForce = 2.6f; 

    // 회전
	float m_rotation = 0.0f;         
	float m_rotationSpeed = 360.0f;  
};

class Background : public GameObject
{
    using BitmapInfo = renderHelp::BitmapInfo;
public:
    Background(const Background&) = delete;
    Background(ObjectType type) : GameObject(type) {}
    ~Background() override;

    void Update(float deltaTime) override;
    void Render(HDC hdc) override;

    void SetBitmapInfo(BitmapInfo* bitmapInfo);

protected:
    void DrawBitmap(HDC hdc);
    // Bitmap 정보
    BitmapInfo* m_pBitmapInfo = nullptr;
};