#pragma once
#include "Component.h";
#include "Vec2.h";

class Transform : public Component {
public:
	Vec2 localPosition = Vec2(0, 0);
	Vec2 localScale = Vec2(1, 1);
	float localRotation = 0;
public:
#pragma region Constructor/Destructor
	Transform();
	virtual ~Transform();
#pragma endregion

#pragma region Global Values
	Vec2 GetGlobalPosition() const;
	Vec2 GetGlobalScale() const;
	float GetGlobalRotation() const;
#pragma endregion

#pragma region Get Set Methods
	void SetPosition(const Vec2 pPos);
	void SetPosition(const float pX, const float pY);
#pragma endregion
};