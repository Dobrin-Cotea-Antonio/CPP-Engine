#include "Collider.h"
#include "../Physics/CollisionManager.h"

#pragma region Constructor/Destructor
Collider::Collider() {
}

Collider::~Collider() {
	if (IsTrigger())
		CollisionManager::GetInstance()->RemoveTriggerCollider(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
	else
		CollisionManager::GetInstance()->RemoveSolidCollider(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
}
#pragma endregion

#pragma region Collider type
void Collider::SetCollisionType(bool pIsTrigger) {
	if (isTrigger == pIsTrigger)
		return;

	isTrigger = pIsTrigger;
	std::weak_ptr<Collider> p = std::dynamic_pointer_cast<Collider>(selfPointer.lock());

	if (isTrigger) {
		CollisionManager::GetInstance()->RemoveSolidCollider(p);
		CollisionManager::GetInstance()->AddTriggerCollider(p);
	}
	else {
		CollisionManager::GetInstance()->RemoveTriggerCollider(p);
		CollisionManager::GetInstance()->AddSolidCollider(p);
	}
}
#pragma endregion

#pragma region Collision Detection
CollisionInfo Collider::GetEarliestCollision(std::weak_ptr<Collider> pOther, Vec2 pVelocity) {
	std::logic_error("Function not yet implemented");
	return CollisionInfo();
}

bool Collider::Overlaps(std::weak_ptr<Collider> pOther) {
	std::logic_error("Function not yet implemented");
	return false;
}
#pragma endregion 

#pragma region Helper Methods
bool Collider::IsTrigger() const {
	return isTrigger;
}

void Collider::FindOwnerCells(Cell* pCellsMatrix[10][10], int pCols, int pRows, Vec2 pCellSize){
}
#pragma endregion