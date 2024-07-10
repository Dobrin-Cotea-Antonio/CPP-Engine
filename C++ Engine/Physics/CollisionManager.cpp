#include "CollisionManager.h"
#include "../Physics/Collider.h"
#include "../Base/Transform.h"
#include "../Base/GameObject.h"
#include "../Base/Game.h"
#include "../Physics/Cell.h"

CollisionManager* CollisionManager::instance = nullptr;

#pragma region Constructor/Destructor
CollisionManager::CollisionManager() {
	partitionedSpace = Vec2(Game::window->getSize().x, Game::window->getSize().y);
	partitionRows = 4;
	partitionCols = 4;

	partitionCellSize = Vec2(partitionedSpace.x / partitionRows, partitionedSpace.y / partitionCols);

	for (int i = 0; i < partitionRows; i++)
		for (int j = 0; j < partitionCols; j++)
			cellsMatrix[i][j] = new Cell();
}

CollisionManager::~CollisionManager() {
}
#pragma endregion

#pragma region Instance
CollisionManager* CollisionManager::GetInstance() {
	if (instance == nullptr)
		instance = new CollisionManager();
	return instance;
}
#pragma endregion

#pragma region Lists
void CollisionManager::AddSolidCollider(const std::weak_ptr<Collider> pCollider) {
	if (ListContainsCollider(solidColliders, pCollider)) {
		std::cout << "List already contains that solid collider\n";
		return;
	}
	solidColliders.push_back(pCollider);

	//UpdateGridCell(pCollider);
	pCollider.lock()->FindOwnerCells(cellsMatrix, partitionCols, partitionRows, partitionCellSize);
}

void CollisionManager::RemoveSolidCollider(const std::weak_ptr<Collider> pCollider) {
	if (!ModifyList(solidColliders, pCollider, true)) {
		std::cout << "List doesnt contain that solid collider\n";
		return;
	}

	for (int i = 0; i < pCollider.lock()->ownerCells.size(); i++)
		ModifyList(pCollider.lock()->ownerCells[i]->solidColliders, pCollider, true);
}

void CollisionManager::AddTriggerCollider(const std::weak_ptr<Collider> pCollider) {
	if (ListContainsCollider(triggerColliders, pCollider)) {
		std::cout << "List already contains that trigger collider\n";
		return;
	}
	triggerColliders.push_back(pCollider);

	pCollider.lock()->FindOwnerCells(cellsMatrix, partitionCols, partitionRows, partitionCellSize);
}

void CollisionManager::RemoveTriggerCollider(const std::weak_ptr<Collider> pCollider) {
	if (!ModifyList(triggerColliders, pCollider, true)) {
		std::cout << "List doesnt contain that trigger collider\n";
		return;
	}

	for (int i = 0; i < pCollider.lock()->ownerCells.size(); i++)
		ModifyList(pCollider.lock()->ownerCells[i]->triggerColliders, pCollider, true);
}
#pragma endregion

#pragma region Helper Methods
bool CollisionManager::ListContainsCollider(const std::vector<std::weak_ptr<Collider>>& pList, const std::weak_ptr<Collider> pCollider) {
	bool wasFound = false;

	for (int i = 0; i < pList.size(); i++)
		if (pList[i].lock().get() == pCollider.lock().get()) {
			wasFound = true;
			break;
		}

	return wasFound;
}

bool CollisionManager::ModifyList(std::vector<std::weak_ptr<Collider>>& pList, const std::weak_ptr<Collider> pCollider, bool pColliderRemove) {
	bool wasFound = false;

	for (int i = 0; i < pList.size(); i++)
		if (pList[i].lock().get() == pCollider.lock().get()) {
			wasFound = true;
			if (pColliderRemove)
				pList.erase(pList.begin() + i);
			break;
		}

	return wasFound;
}
#pragma endregion

#pragma region Move
void CollisionManager::Move(const std::weak_ptr<Collider> pCollider, const Vec2 pVelocity) {
	if (pVelocity.Length() == 0)
		return;

	Transform* colliderTransform = pCollider.lock()->owner.lock()->transform.lock().get();
	colliderTransform->localPosition += pVelocity;
	//UpdateGridCell(pCollider);
	pCollider.lock()->FindOwnerCells(cellsMatrix, partitionCols, partitionRows, partitionCellSize);

	if (pCollider.lock()->IsTrigger())
		CalculateOverlaps(pCollider, pVelocity);
	else
		CalculateCollisions(pCollider, pVelocity);
}

void CollisionManager::CalculateOverlaps(const std::weak_ptr<Collider> pCollider, const Vec2 pVelocity) {

	if (!useSpacePartitioning) {
		FinaliseOverlaps(pCollider, triggerColliders, pVelocity);
		return;
	}

	if (pCollider.lock()->ownerCells.size() == 0)
		return;

	std::vector<std::weak_ptr<Collider>> collidersToCheck;

	for (int i = 0; i < pCollider.lock()->ownerCells.size(); i++)
		for (int j = 0; j < pCollider.lock()->ownerCells[i]->triggerColliders.size(); j++) {
			collidersToCheck.push_back(pCollider.lock()->ownerCells[i]->triggerColliders[j]);
		}

	FinaliseCollisions(pCollider, collidersToCheck, pVelocity);
}
void CollisionManager::FinaliseOverlaps(const std::weak_ptr<Collider> pCollider, std::vector<std::weak_ptr<Collider>>& pList, const Vec2 pVelocity) {
	for (int i = 0; i < pList.size(); i++) {
		//std::cout << "TEST1\n";
		if (pCollider.lock().get() == pList[i].lock().get())
			continue;
		if (!pCollider.lock()->Overlaps(pList[i]))
			continue;

		//<----------------------------------------------------------------->//
		//message both objects that they are overlaping						 //
		//<----------------------------------------------------------------->//
	}
}
void CollisionManager::CalculateCollisions(const std::weak_ptr<Collider> pCollider, const Vec2 pVelocity) {
	if (!useSpacePartitioning) {
		FinaliseCollisions(pCollider, solidColliders, pVelocity);
		return;
	}

	if (pCollider.lock()->ownerCells.size() == 0)
		return;

	std::vector<std::weak_ptr<Collider>> collidersToCheck;

	for (int i = 0; i < pCollider.lock()->ownerCells.size(); i++)
		for (int j = 0; j < pCollider.lock()->ownerCells[i]->solidColliders.size(); j++) {
			collidersToCheck.push_back(pCollider.lock()->ownerCells[i]->solidColliders[j]);
		}

	FinaliseCollisions(pCollider, collidersToCheck, pVelocity);
}

void CollisionManager::FinaliseCollisions(const std::weak_ptr<Collider> pCollider, std::vector<std::weak_ptr<Collider>>& pList, const Vec2 pVelocity) {
	CollisionInfo firstCollision;
	Transform* colliderTransform = pCollider.lock()->owner.lock()->transform.lock().get();

	for (int i = 0; i < pList.size(); i++) {
		if (pCollider.lock().get() == pList[i].lock().get())
			continue;

		CollisionInfo collisionInfo = pCollider.lock()->GetEarliestCollision(pList[i], pVelocity);//solid

		if (collisionInfo.timeOfImpact == -1 || collisionInfo.timeOfImpact >= 1)
			continue;

		if (firstCollision.timeOfImpact == -1 || firstCollision.timeOfImpact > collisionInfo.timeOfImpact)
			firstCollision = collisionInfo;
	}

	float timeOfImpact = 1;

	if (firstCollision.timeOfImpact == -1 || firstCollision.timeOfImpact > 1 || firstCollision.timeOfImpact < 0)
		return;

	timeOfImpact = firstCollision.timeOfImpact;

	colliderTransform->localPosition -= pVelocity;
	colliderTransform->localPosition += pVelocity * timeOfImpact;

	//<----------------------------------------------------------------->//
	//message both objects that they are colliding						 //
	//<----------------------------------------------------------------->//
}
#pragma endregion

#pragma region Space Partitioning
void CollisionManager::EnableSpacePartitioning(const bool pState) {
	useSpacePartitioning = pState;
}
#pragma endregion