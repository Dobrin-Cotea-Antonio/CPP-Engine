#pragma once
#include "Object.h"

class GameObject;

class Component : public Object {
public:
	std::weak_ptr<GameObject> owner;

protected:
#pragma region Special Events
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnCreate();
	virtual void OnDestroy();
#pragma endregion 
public:

#pragma region Constructor/Destructor
	Component();
	virtual ~Component();
#pragma endregion

#pragma region Runtime
	virtual void Update();
	virtual void update();
	virtual void render();
#pragma endregion

#pragma region State
	void Destroy() override;
	void Enable(const bool pState) override;
#pragma endregion

#pragma region Utility
	bool IsEnabledGlobal() override;
	//void SetSelfPointer(std::weak_ptr<Object> pSelfPointer);
#pragma endregion

};