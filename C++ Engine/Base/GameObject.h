#pragma once
#include <vector>
#include <iostream>
#include "Object.h"

class Component;
class Transform;
class CollisionManager;

class GameObject : public Object {
	friend CollisionManager;

private:
	std::vector<std::shared_ptr<Component>> components;

protected:
	std::vector<std::weak_ptr<GameObject>> children;
	std::weak_ptr<GameObject> parent;
	std::weak_ptr<GameObject> selfPointer;

public:
	std::weak_ptr<Transform> transform;
public:

#pragma region Constructor/Destructor
	GameObject();
	virtual ~GameObject();
#pragma endregion

#pragma region Hierarchy
	void AddChild(std::weak_ptr<GameObject> pGameObject);
	void RemoveChild(std::weak_ptr<GameObject> pGameObject);
	void RemoveParent();
#pragma endregion

#pragma region Runtime
	void update();
	void Update();
	void render();
#pragma endregion

#pragma region Utility
	bool IsEnabledGlobal() override;
	void SetSelfPointer(std::weak_ptr<GameObject> pSelfPointer);
#pragma endregion

#pragma region State
	void Destroy();
#pragma endregion

#pragma region Components
	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	std::weak_ptr<T> AddComponent(std::shared_ptr<T> pComponent) {
		bool wasFound = false;

		//check if the component was already assigned to a different GameObject


		for (int i = 0; i < components.size(); i++)
			if (pComponent.get() == components[i].get()) {
				wasFound = true;
				break;
			}

		if (!wasFound) {
			components.push_back(pComponent);
			pComponent->owner = selfPointer;
		}

		else
			std::cout << "Component was already assigned to this GameObject!";

		return pComponent;
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	void RemoveComponent(std::weak_ptr<T> pComponent) {
		bool wasFound = false;

		for (int i = 0; i < components.size(); i++)
			if (pComponent.lock().get() == components[i].get()) {
				components.erase(components.begin() + i);
				wasFound = true;
				break;
			}

		if (!wasFound)
			std::cout << "Component isn`t assigned to this GameObject!";
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	std::weak_ptr<T> GetComponentOfType() {

		for (int i = 0; i < components.size(); i++) {
			T* comp = dynamic_cast<T*>(components[i].get());
			if (comp != nullptr)
				return std::dynamic_pointer_cast<T>(components[i]);
		}
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	std::vector<std::weak_ptr<T>> GetComponentsOfType() {

		std::vector<std::weak_ptr<T>> componentList;

		for (int i = 0; i < components.size(); i++) {
			T* comp = dynamic_cast<T*>(components[i].get());
			if (comp != nullptr)
				componentList.push_back(components[i]);
		}

		return componentList;
	}

#pragma endregion
};