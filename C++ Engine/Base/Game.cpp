#include "Game.h"
#include "SceneManager.h"
#include "TimeClass.h"
#include "TextureCache.h"
#include "../Base/InputManager.h"
#include <iostream>

sf::RenderWindow* Game::window = nullptr;

#pragma region Constructor/Destructor
Game::Game(const Vec2 pWindowSize) {
	sceneManager = SceneManager::GetInstance();
	textureCache = TextureCache::GetInstance();
	timeClass = TimeClass::GetInstance();
	inputManager = InputManager::GetInstance();
	window = new sf::RenderWindow(sf::VideoMode(pWindowSize.x, pWindowSize.y), "Yes");
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);
}

Game::~Game() {
}
#pragma endregion

#pragma region Runtime
void Game::run() {
	while (window->isOpen()) {
		sf::Event event;

		inputManager->StartUpdate();

		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			inputManager->update(event);
		}

		timeClass->update();

		sceneManager->update();
		sceneManager->Update();
		sceneManager->DestroyObjects();
		
		window->clear();
		sceneManager->render();
		window->display();
	}
}
#pragma endregion

#pragma region Game
void Game::CreateGame() {
}
#pragma endregion
