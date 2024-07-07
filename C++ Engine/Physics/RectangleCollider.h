#pragma once
#include "../Physics/Collider.h"
#include "../Base/Vec2.h";
#include "../Physics/CollisionInfo.h"

class CircleCollider;

class RectangleCollider :public Collider {
private:
	// top left / top right / bottom right / bottom left 
	Vec2 vertices[4] = { Vec2(0,0),Vec2(0,0) ,Vec2(0,0) ,Vec2(0,0) };
	Vec2 verticesGlobal[4] = { Vec2(0,0),Vec2(0,0) ,Vec2(0,0) ,Vec2(0,0) };

#pragma region Constructor/Destructor
public:
	RectangleCollider();
	virtual ~RectangleCollider();
#pragma endregion

#pragma region Collision Detection
public:
	virtual CollisionInfo GetEarliestCollision(const std::weak_ptr<Collider> pOther, const Vec2 pVelocity)override;
	virtual bool Overlaps(std::weak_ptr<Collider> pOther)override;
private:
	CollisionInfo GetEarliestCollision(RectangleCollider* pOther, const Vec2 pVelocity);
	CollisionInfo GetEarliestCollision(CircleCollider* pOther, const Vec2 pVelocity);
	bool Overlaps(RectangleCollider* pOther)const;
	bool Overlaps(CircleCollider* pOther)const;
#pragma endregion 

#pragma region Helper Methods
private:
	float CalculateCircleTimeOfImpact(const RectangleCollider* pOther, const Vec2 pVelocity);
	void GetGlobalBounds();
public:
	void SetBounds(const float pWidth, const float pHeight);
#pragma endregion
};