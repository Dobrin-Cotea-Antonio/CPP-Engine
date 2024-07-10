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
	bool Overlaps(RectangleCollider* pOther);
	bool Overlaps(CircleCollider* pOther);
#pragma endregion 

#pragma region Helper Methods
private:
	float CalculateCircleTimeOfImpact(const RectangleCollider* pOther, const Vec2 pVelocity);
	void GetGlobalBounds();

	void ReturnNormalsAxis(std::vector<Vec2>& pNormals, RectangleCollider* pRect1, RectangleCollider* pRect2);
	Vec2 ReturnCircleAxis(CircleCollider* pCircle, RectangleCollider* pRect);
	Vec2 ProjectOntoAxis(RectangleCollider* pCollider, Vec2 pAxis)const;
	Vec2 ProjectOntoAxis(std::vector<Vec2>& pVertices, Vec2 pAxis)const;
	Vec2 ProjectCircleOntoAxis(CircleCollider* pCircle, Vec2 pAxis)const;
	float GetRectOverlap(const Vec2 pRect1Values, const Vec2 pRect2Values)const;
public:
	void SetBounds(const float pWidth, const float pHeight);
	void ReturnGlobalBounds(std::vector<Vec2>& pVerticesGlobal);
	Vec2 ReturnGlobalCenter()const;

#pragma endregion
};