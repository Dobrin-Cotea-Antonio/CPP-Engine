#pragma once
#include <string>
#include <map>
#include <memory>

#include "Scene.h"

class SceneManager {
	friend class Game;

private:
	std::map<std::string, std::shared_ptr<Scene>> scenes;
	std::string activeScene;

public:
	static SceneManager* instance;
public:

#pragma region Constructor/Destructor
	SceneManager();
	virtual ~SceneManager();
#pragma endregion

#pragma region Deleted Methods
	SceneManager(SceneManager& pOther) = delete;
	void operator=(const SceneManager&) = delete;
#pragma endregion
	//make static
#pragma region Add Scene Template
	template<typename T, typename = std::enable_if_t<std::is_base_of<Scene, T>::value>>
	std::weak_ptr<T> AddScene(std::string pID) {
		//scenes[pID] = std::shared_ptr<T>(new T(pID));
		scenes[pID] = std::make_shared<T>(T(pID));
		scenes[pID]->SetLoadScene([this](std::string pScene) {
			EnableScene(pScene);
			});

		if (activeScene.empty())
			EnableScene(pID);

		std::weak_ptr<T> ptr = std::dynamic_pointer_cast<T>(scenes[pID]);
		return ptr;
	}
#pragma endregion

#pragma region Runtime
private:
	void update();
	void Update();
	void render();
	void DestroyObjects();
#pragma endregion

#pragma region Helper Methods
public:
	void EnableScene(const std::string pID);
	std::string GetActiveScene();
	static SceneManager* GetInstance();
	static std::weak_ptr<Scene> ReturnActiveScene();
#pragma endregion

};