#include "Scene.h"
#include "GameObject.h"

#pragma region Constructor/Destructor
Scene::Scene(const std::string pID) :ID(pID) {}

Scene::~Scene() {}
#pragma endregion

#pragma region Runtime
void Scene::update() {
	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->IsEnabledGlobal())
			objects[i]->update();
}
void Scene::Update() {
	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->IsEnabledGlobal())
			objects[i]->Update();
}

void Scene::DestroyObjects() {
	for (int i = objects.size() - 1; i >= 0; i--) {
		if (objects[i]->GetDestroyStatus()) {
			objects[i].reset();//might cause memory leak
			objects.erase(objects.begin() + i);
		}
	}
}

void Scene::render() const {
	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->IsEnabledGlobal())
			objects[i]->render();
}
#pragma endregion

#pragma region Scene Functionality
void Scene::CreateLevel() {
}

void Scene::DestroyScene() {
	objects.clear();//might cause memory leak
}

void Scene::SetLoadScene(std::function<void(std::string pScene)> pLoadScene) {
	loadScene = pLoadScene;
}

void Scene::LoadScene(const std::string pScene) {
	loadScene(pScene);
}
#pragma endregion

#pragma region Helper Methods
const std::string Scene::GetID() const {
	return ID;
}
#pragma endregion
