#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

SceneManager* SceneManager::instance = nullptr;

#pragma region Constructor/Destructor
SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}
#pragma endregion

#pragma region Runtime
void SceneManager::update() {
	if (activeScene.empty())
		return;
	scenes[activeScene]->update();
}

void SceneManager::Update() {
	if (activeScene.empty())
		return;
	scenes[activeScene]->Update();
}

void SceneManager::render() {
	if (activeScene.empty())
		return;
	scenes[activeScene]->render();
}

void SceneManager::DestroyObjects() {
	if (activeScene.empty())
		return;
	scenes[activeScene]->DestroyObjects();
}
#pragma endregion

#pragma region Helper Methods
void SceneManager::EnableScene(const std::string pID) {
	if (!activeScene.empty())
		scenes[activeScene]->DestroyScene();
	activeScene = pID;
	scenes[activeScene]->CreateLevel();
}

std::string SceneManager::GetActiveScene() {
	return activeScene;
}

SceneManager* SceneManager::GetInstance() {
	if (instance == nullptr)
		instance = new SceneManager();
	return instance;
}
std::weak_ptr<Scene> SceneManager::ReturnActiveScene() {
	return instance->scenes[instance->activeScene];
}
#pragma endregion
