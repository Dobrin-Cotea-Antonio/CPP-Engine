#pragma once
#include<vector>
#include<iostream>
#include<functional>

class GameObject;
class Component;

class Scene {
	friend class SceneManager;
protected:
	std::vector<std::shared_ptr<GameObject>> objects;
	std::function<void(std::string pScene)> loadScene;
private:
	const std::string ID;
public:
#pragma region Constructor/Destructor
	Scene(const std::string pID);
	virtual ~Scene();
#pragma endregion

#pragma region Runtime
protected:
	virtual void update();
	virtual void Update();
	void DestroyObjects();
	void render()const;
#pragma endregion

#pragma region Instantiate Template
public:
	template<typename T, typename = std::enable_if_t<std::is_base_of<GameObject, T>::value>>
	std::weak_ptr<T> InstantiateGameObject() {
		objects.push_back(std::shared_ptr<T>(new T()));
		std::weak_ptr<T>ptr = std::dynamic_pointer_cast<T>(objects[objects.size() - 1]);
		ptr.lock()->SetSelfPointer(ptr);
		return ptr;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	std::shared_ptr<T> InstantiateComponent() {
		std::shared_ptr<T> pointer = std::shared_ptr<T>(new T());
		std::weak_ptr<T>ptr = std::dynamic_pointer_cast<T>(pointer);
		pointer->SetSelfPointer(ptr);
		return pointer;
	}
#pragma endregion

#pragma region Scene Functionality
	virtual void CreateLevel();
	void DestroyScene();
	void SetLoadScene(std::function<void(std::string pScene)> pLoadScene);

private:
	void LoadScene(const std::string pScene);
#pragma endregion

#pragma region Helper Methods
public:
	const std::string GetID()const;
#pragma endregion


};