#pragma once
#include "../Base/Scene.h";

class TestScene : public Scene {

	friend class SceneManager;

#pragma region Constructor/Destructor
public:
	TestScene();
	TestScene(const std::string pID);
	virtual ~TestScene();
#pragma endregion

#pragma region Scene Functionality
	void CreateLevel()override;
#pragma endregion
};