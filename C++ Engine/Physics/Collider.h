#pragma once
#include "../Base/Component.h"
#include "../Base/Vec2.h"
#include "../Physics/CollisionInfo.h"
#include <vector>

class Cell;

class Collider :public Component {
protected:
	bool isTrigger = true;
public:
	std::vector<Cell*> ownerCells;

#pragma region Constructor/Destructor
public:
	Collider();
	virtual ~Collider();
#pragma endregion

#pragma region Collider type
public:
	void SetCollisionType(bool pIsTrigger);
#pragma endregion

#pragma region Collision Detection
public:
	virtual CollisionInfo GetEarliestCollision(std::weak_ptr<Collider> pOther, Vec2 pVelocity) = 0;
	virtual bool Overlaps(std::weak_ptr<Collider> pOther) = 0;
#pragma endregion 

#pragma region Helper Methods
public:
	bool IsTrigger()const;
	virtual void FindOwnerCells(Cell* pCellsMatrix[10][10],int pCols,int pRows,Vec2 pCellSize);
#pragma endregion
};