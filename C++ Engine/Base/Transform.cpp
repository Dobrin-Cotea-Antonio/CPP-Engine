#include "Transform.h"
#include "GameObject.h"

#pragma region Constructor/Destructor
Transform::Transform() :Component() { }

Transform::~Transform() { }
#pragma endregion

#pragma region Global Values
Vec2 Transform::GetGlobalPosition() const {
	if (owner.lock() == nullptr)
		return localPosition;
	else
		return localPosition + owner.lock()->transform.lock()->GetGlobalPosition();
}

Vec2 Transform::GetGlobalScale() const {
	if (owner.lock() == nullptr)
		return localScale;
	else {
		Vec2 parentScale = owner.lock()->transform.lock()->GetGlobalScale();
		return Vec2(localScale.x * parentScale.x, localScale.y * parentScale.y);
	}
}

float Transform::GetGlobalRotation() const {
	if (owner.lock() == nullptr)
		return localRotation;
	else
		return localRotation + owner.lock()->transform.lock()->GetGlobalRotation();
}
#pragma endregion

#pragma region Get Set Methods
void Transform::SetPosition(const Vec2 pPos) {
	localPosition = pPos;
}

void Transform::SetPosition(const float pX, const float pY) {
	localPosition.x = pX;
	localPosition.y = pY;
}
#pragma endregion