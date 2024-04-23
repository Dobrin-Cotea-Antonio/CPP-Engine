#include "TestScene.h"
#include "../Base/SpriteRenderer.h"
#include "../Base/GameObject.h"
#include "../Base/Transform.h"

#pragma region Constructor/Destructor
TestScene::TestScene(const std::string pID) :Scene(pID) {
}

TestScene::~TestScene() {
}
#pragma endregion

#pragma region Scene Functionality
void TestScene::CreateLevel() {
	//InstantiateGameObject<GameObject>();
}
#pragma endregion