#pragma once
#include "../Base/Component.h"

class Collider;

class CollisionTest :public Component {
private:
	std::weak_ptr<Collider> collider;
	float speed = 150;;//pixels per sec
	float rotationSpeed = 90;//deg per sec

#pragma region Constructor/Destructor
public:
	CollisionTest();
	virtual ~CollisionTest();
#pragma endregion

#pragma region runtime
public:
	virtual void Update()override;
#pragma endregion

};

