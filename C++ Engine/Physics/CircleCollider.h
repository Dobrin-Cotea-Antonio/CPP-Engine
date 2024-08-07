#pragma once
#include "../Physics/Collider.h"
#include "../Base/Vec2.h";
#include "../Physics/CollisionInfo.h"

class RectangleCollider;

class CircleCollider :public Collider {
public:
	float radius = 1;

#pragma region Constructor/Destructor
public:
	CircleCollider();
	virtual ~CircleCollider();
#pragma endregion

#pragma region Collision Detection
public:
	virtual CollisionInfo GetEarliestCollision(const std::weak_ptr<Collider> pOther, const Vec2 pVelocity)override;
	virtual bool Overlaps(std::weak_ptr<Collider> pOther)override;
private:
	CollisionInfo GetEarliestCollision(CircleCollider* pOther, const Vec2 pVelocity);
	CollisionInfo GetEarliestCollision(RectangleCollider* pOther, const Vec2 pVelocity);
	bool Overlaps(CircleCollider* pOther)const;
	bool Overlaps(RectangleCollider* pOther)const;
#pragma endregion 

#pragma region Helper Methods
private:
	float CalculateCircleTimeOfImpact(const CircleCollider* pOther, const Vec2 pVelocity);
	float CalculateDistanceToLine(Vec2 pLineStart, Vec2 pLineEnd);
public:
	virtual void FindOwnerCells(Cell* pCellsMatrix[10][10], int pCols, int pRows, Vec2 pCellSize)override;
#pragma endregion
};
