#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"

class SceneManager;
class TextureCache;
class TimeClass;
class InputManager;

class Game {
protected:
	SceneManager* sceneManager;
	TextureCache* textureCache;
	TimeClass* timeClass;
	InputManager* inputManager;
public:
	static sf::RenderWindow* window;
public:
#pragma region Constructor/Destructor
	Game(const Vec2 pWindowSize);
	virtual ~Game();
#pragma endregion

#pragma region Runtime
	void run();
#pragma endregion

#pragma region Game
protected:
	virtual void CreateGame();
#pragma endregion
};