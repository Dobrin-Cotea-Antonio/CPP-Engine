#include "CollisionTest.h"
#include "../Base/GameObject.h"
#include "../Physics/Collider.h"
#include "../Base/InputManager.h"
#include "../Physics/CollisionManager.h"
#include "../Base/TimeClass.h"
#include "../Base/Transform.h"

#pragma region Constructor/Destructor
CollisionTest::CollisionTest() {

}

CollisionTest::~CollisionTest() {
}
#pragma endregion

#pragma region runtime
void CollisionTest::Update() {

	if (collider.lock().get() == nullptr)
		collider = owner.lock()->GetComponentOfType<Collider>();

	Vec2 direction;

	if (InputManager::IsKeyPressed(Key::W)) 
		direction += Vec2(0, -1);
	if (InputManager::IsKeyPressed(Key::A))
		direction += Vec2(-1, 0);
	if (InputManager::IsKeyPressed(Key::S))
		direction += Vec2(0, 1);
	if (InputManager::IsKeyPressed(Key::D))
		direction += Vec2(1, 0);

	if (InputManager::IsKeyPressed(Key::LeftArrow))
		owner.lock()->transform.lock()->localRotation -= TimeClass::deltaTime * rotationSpeed;

	if (InputManager::IsKeyPressed(Key::RightArrow))
		owner.lock()->transform.lock()->localRotation += TimeClass::deltaTime * rotationSpeed;
	
	direction.Normalize();

	CollisionManager::GetInstance()->Move(collider, speed * TimeClass::deltaTime * direction);
}
#pragma endregion
