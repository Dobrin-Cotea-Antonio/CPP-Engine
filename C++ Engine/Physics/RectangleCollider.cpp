#include "RectangleCollider.h"
#include "../Base/GameObject.h"
#include "../Base/Transform.h"
#include "../Physics/CircleCollider.h"

#pragma region Constructor/Destructor
RectangleCollider::RectangleCollider() {}

RectangleCollider::~RectangleCollider() {}
#pragma endregion

#pragma region Collision Detection
CollisionInfo RectangleCollider::GetEarliestCollision(const std::weak_ptr<Collider> pOther, const Vec2 pVelocity) {
	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return GetEarliestCollision(circle.get(), pVelocity);

	std::shared_ptr<RectangleCollider> rectangle = std::dynamic_pointer_cast<RectangleCollider>(pOther.lock());

	if (rectangle.get() != nullptr)
		return GetEarliestCollision(rectangle.get(), pVelocity);

	return CollisionInfo();
}

bool RectangleCollider::Overlaps(std::weak_ptr<Collider> pOther) {
	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return Overlaps(circle.get());

	std::shared_ptr<RectangleCollider> rectangle = std::dynamic_pointer_cast<RectangleCollider>(pOther.lock());

	if (rectangle.get() != nullptr)
		return Overlaps(rectangle.get());

	return false;
}

CollisionInfo RectangleCollider::GetEarliestCollision(RectangleCollider* pOther, const Vec2 pVelocity) {
	return CollisionInfo();
}

CollisionInfo RectangleCollider::GetEarliestCollision(CircleCollider* pOther, const Vec2 pVelocity) {
	return CollisionInfo();
}

bool RectangleCollider::Overlaps(RectangleCollider* pOther) const {
	return false;
}

bool RectangleCollider::Overlaps(CircleCollider* pOther) const {
	return false;
}

#pragma endregion

#pragma region Helper Methods
void RectangleCollider::GetGlobalBounds() {
	//bounding box with rotation

	Transform* transform = owner.lock()->transform.lock().get();
	Vec2 globalPosition = transform->GetGlobalPosition();
	float globalRotation = transform->GetGlobalRotation();

	for (int i = 0; i < 4; i++) {
		verticesGlobal[i] = vertices[i] + globalPosition;
		verticesGlobal[i].RotateAroundDegrees(globalPosition, globalRotation);
	}
}

void RectangleCollider::SetBounds(const float pWidth, const float pHeight) {
	//bounding box without rotation

	Transform* transform = owner.lock()->transform.lock().get();

	vertices[0] = Vec2(-pWidth / 2, -pHeight / 2);
	vertices[1] = Vec2(pWidth / 2, -pHeight / 2);
	vertices[2] = Vec2(pWidth / 2, pHeight / 2);
	vertices[3] = Vec2(-pWidth / 2, pHeight / 2);
}


#pragma endregion
