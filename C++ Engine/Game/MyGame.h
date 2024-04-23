#pragma once
#include "../Base/Game.h"

class MyGame :public Game {

#pragma region Constructor/Destructor
public:
	MyGame(const Vec2 pWindowSize);
	virtual ~MyGame();
#pragma endregion

#pragma region Game
public:
	virtual void CreateGame()override;
#pragma endregion
};
