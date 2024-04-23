#include "CircleCollider.h"
#include "../Base/GameObject.h"
#include "../Base/Transform.h"
#include <cmath>
#include "../Physics/Cell.h"

#pragma region Constructor/Destructor
CircleCollider::CircleCollider() {
}

CircleCollider::~CircleCollider() {
}
#pragma endregion

#pragma region Collision Detection
CollisionInfo CircleCollider::GetEarliestCollision(std::weak_ptr<Collider> pOther, const Vec2 pVelocity) {
	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return GetEarliestCollision(circle.get(), pVelocity);

	return CollisionInfo();
}

bool CircleCollider::Overlaps(std::weak_ptr<Collider> pOther) {
	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return Overlaps(circle);

	return false;
}

CollisionInfo CircleCollider::GetEarliestCollision(CircleCollider* pOther, const Vec2 pVelocity) {
	Vec2 relativePosition = owner.lock()->transform.lock()->GetGlobalPosition() - pOther->owner.lock()->transform.lock()->GetGlobalPosition();
	if (relativePosition.Length() < radius + pOther->radius) {

		Vec2 oldPosition = owner.lock()->transform.lock()->GetGlobalPosition() - pVelocity;
		float timeOfImpact = CalculateCircleTimeOfImpact(pOther, pVelocity);

		if (timeOfImpact < 0 || timeOfImpact > 1)
			return CollisionInfo();

		Vec2 POI = oldPosition + pVelocity * timeOfImpact;
		Vec2 unitNormal = (POI - pOther->owner.lock()->transform.lock()->GetGlobalPosition()).Normalized();
		return CollisionInfo(unitNormal, pOther, timeOfImpact);
	}
	return CollisionInfo();
}

bool CircleCollider::Overlaps(std::weak_ptr<CircleCollider> pOther, Vec2 pVelocity) const {
	Vec2 diffVec = owner.lock()->transform.lock()->GetGlobalPosition() - pOther.lock()->owner.lock()->transform.lock()->GetGlobalPosition();
	return (diffVec.Length() < radius + pOther.lock()->radius);
}
#pragma endregion

#pragma region Helper Methods
float CircleCollider::CalculateCircleTimeOfImpact(const CircleCollider* pOther, const Vec2 pVelocity) {
	Vec2 u = ((owner.lock()->transform.lock()->GetGlobalPosition() - pVelocity) - pOther->owner.lock()->transform.lock()->GetGlobalPosition());

	float a = pow(pVelocity.Length(), 2);
	float b = 2 * u.Dot(pVelocity);
	float c = pow(u.Length(), 2) - pow(radius + pOther->radius, 2);
	float D = pow(b, 2) - 4 * a * c;

	if (c < 0) {
		if (b < 0)
			return 0;
		else
			return -1;
	}

	if (a == 0)
		return -1;

	if (D < 0)
		return -1;

	float t = (-b - sqrt(D)) / (2 * a);

	if (0 <= t && t < 1)
		return t;

	return -1;
}

float CircleCollider::CalculateDistanceToLine(Vec2 pLineStart, Vec2 pLineEnd) {// returns distance or -1
	Vec2 lineVector = pLineEnd - pLineStart;
	Vec2 differenceVector = owner.lock()->transform.lock()->GetGlobalPosition() - pLineStart;

	float projection = differenceVector.Dot(lineVector);
	float d = projection / (pow(lineVector.Length(), 2));

	//std::cout << lineVector;

	//std::cout << projection<<" "<< pow(lineVector.Length(), 2) <<" " << d << "\n";

	if (d < 0)
		return -1;
	if (d > 1)
		return -1;

	Vec2 point = pLineStart + lineVector * d;

	//std::cout << (point - owner.lock()->transform.lock()->GetGlobalPosition()).Length() << "\n";
	return (point - owner.lock()->transform.lock()->GetGlobalPosition()).Length();

}

void CircleCollider::FindOwnerCells(Cell* pCellsMatrix[10][10], int pCols, int pRows, Vec2 pCellSize) {

	std::shared_ptr<Collider> colPointer = std::dynamic_pointer_cast<Collider>(selfPointer.lock());

	for (int i = 0; i < ownerCells.size(); i++) {

		if (IsTrigger()) {
			for (int j = 0; j < ownerCells[i]->triggerColliders.size(); j++)
				if (ownerCells[i]->triggerColliders[j].lock().get() == colPointer.get()) {
					ownerCells[i]->triggerColliders.erase(ownerCells[i]->triggerColliders.begin() + j);
					break;
				}
		}
		else {
			for (int j = 0; j < ownerCells[i]->solidColliders.size(); j++)
				if (ownerCells[i]->solidColliders[j].lock().get() == colPointer.get()) {
					ownerCells[i]->solidColliders.erase(ownerCells[i]->solidColliders.begin() + j);
					break;
				}
		}
	}

	ownerCells.clear();

	for (int i = 0; i < pCols; i++)
		for (int j = 0; j < pRows; j++) {
			Vec2 cellTopLeft(pCellSize.x * i, pCellSize.y * j);
			Vec2 cellTopRight(pCellSize.x * (i + 1), pCellSize.y * j);
			Vec2 cellBottomLeft(pCellSize.x * i, pCellSize.y * (j + 1));
			Vec2 cellBottomRight(pCellSize.x * (i + 1), pCellSize.y * (j + 1));

			//std::cout << "Cell " << i << " " << j << "\n";
			//std::cout << cellTopLeft;
			//std::cout << cellTopRight;
			//std::cout << cellBottomRight;
			//std::cout << cellBottomLeft;

			float distance = CalculateDistanceToLine(cellTopLeft, cellTopRight);
			if (distance != -1 && distance < radius) {
				ownerCells.push_back(pCellsMatrix[i][j]);

				if (IsTrigger())
					pCellsMatrix[i][j]->triggerColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
				else
					pCellsMatrix[i][j]->solidColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));

				continue;
			}

			distance = CalculateDistanceToLine(cellTopRight, cellBottomRight);
			if (distance != -1 && distance < radius) {
				ownerCells.push_back(pCellsMatrix[i][j]);

				if (IsTrigger())
					pCellsMatrix[i][j]->triggerColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
				else
					pCellsMatrix[i][j]->solidColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));

				continue;
			}

			distance = CalculateDistanceToLine(cellBottomRight, cellBottomLeft);
			if (distance != -1 && distance < radius) {
				ownerCells.push_back(pCellsMatrix[i][j]);

				if (IsTrigger())
					pCellsMatrix[i][j]->triggerColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
				else
					pCellsMatrix[i][j]->solidColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));

				continue;
			}

			distance = CalculateDistanceToLine(cellBottomLeft, cellTopLeft);
			if (distance != -1 && distance < radius) {
				ownerCells.push_back(pCellsMatrix[i][j]);

				if (IsTrigger())
					pCellsMatrix[i][j]->triggerColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
				else
					pCellsMatrix[i][j]->solidColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));

				continue;
			}

		}

	if (ownerCells.size() == 0) {
		Vec2 position = owner.lock()->transform.lock()->GetGlobalPosition();
		int posX = position.x / pCellSize.x;
		int posY = position.y / pCellSize.y;

		if (posX < 0 || posX >= pRows || posY < 0 || posY >= pCols)
			std::cout << "Collider is outside of the cell range " << posX << " " << posY << "\n";
		else {
			ownerCells.push_back(pCellsMatrix[posX][posY]);

			if (IsTrigger())
				pCellsMatrix[posX][posY]->triggerColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
			else
				pCellsMatrix[posX][posY]->solidColliders.push_back(std::dynamic_pointer_cast<Collider>(selfPointer.lock()));
		}

	}

}
#pragma endregion