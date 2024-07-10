#include "MyGame.h";
#include "../Base/SceneManager.h";
#include "../Game/TestScene.h";
#include "../Base/AnimatedSpriteRenderer.h";
#include "../Base/GameObject.h";
#include "../Base/Transform.h";
#include "../Base/SpriteRenderer.h";
#include <iostream>
#include "../Base/AnimationData.h"
#include "../Game/CollisionTest.h"
#include "../Physics/CircleCollider.h"
#include "../Physics/CollisionManager.h"
#include "../Physics/RectangleCollider.h"

#include "../Game/EventTest.h"

#pragma region Constructor/Destructor
MyGame::MyGame(const Vec2 pWindowSize) :Game(pWindowSize) {
	CreateGame();
}

MyGame::~MyGame(){
}
#pragma endregion

#pragma region Game

void MyGame::CreateGame() {

	CollisionManager::GetInstance()->EnableSpacePartitioning(false);

	std::weak_ptr<Scene> scene = sceneManager->AddScene<Scene>("test");

	std::weak_ptr<GameObject> g = scene.lock()->InstantiateGameObject<GameObject>();
	std::weak_ptr<SpriteRenderer> s = g.lock()->AddComponent(scene.lock()->InstantiateComponent<SpriteRenderer>());
	s.lock()->LoadSprite("Game/Assets/rectangle.png");

	g.lock()->transform.lock()->localPosition = Vec2(100, 100);
	std::weak_ptr<RectangleCollider> collider = g.lock()->AddComponent(scene.lock()->InstantiateComponent<RectangleCollider>());
	collider.lock()->SetBounds(64, 64);
	collider.lock()->SetCollisionType(false);

	std::weak_ptr<CollisionTest> collisionTest = g.lock()->AddComponent(scene.lock()->InstantiateComponent<CollisionTest>());


	std::weak_ptr<GameObject> g1 = scene.lock()->InstantiateGameObject<GameObject>();
	std::weak_ptr<SpriteRenderer> s1 = g1.lock()->AddComponent(scene.lock()->InstantiateComponent<SpriteRenderer>());
	s1.lock()->LoadSprite("Game/Assets/rectangle.png");

	g1.lock()->transform.lock()->localPosition = Vec2(300, 300);
	std::weak_ptr<RectangleCollider> collider1 = g1.lock()->AddComponent(scene.lock()->InstantiateComponent<RectangleCollider>());
	collider1.lock()->SetBounds(64, 64);
	collider1.lock()->SetCollisionType(false);



	//std::weak_ptr<GameObject> g2 = scene.lock()->InstantiateGameObject<GameObject>();
	//std::weak_ptr<SpriteRenderer> s2 = g2.lock()->AddComponent(scene.lock()->InstantiateComponent<SpriteRenderer>());
	//s2.lock()->LoadSprite("Game/Assets/circle.png");

	//g2.lock()->transform.lock()->localPosition = Vec2(400, 400);
	//std::weak_ptr<CircleCollider> collider2 = g2.lock()->AddComponent(scene.lock()->InstantiateComponent<CircleCollider>());
	//collider2.lock()->radius = 32;
	//collider2.lock()->SetCollisionType(true);

	//EventTest test;

	std::weak_ptr<EventTest> test = g.lock()->AddComponent(scene.lock()->InstantiateComponent<EventTest>());
}
#pragma endregion
