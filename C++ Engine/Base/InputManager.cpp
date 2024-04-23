#include "InputManager.h"
#include "../Base/Game.h"
#include <iostream>

InputManager* InputManager::instance = nullptr;

bool InputManager::keyPressed[Key::KeyCount] = {};
bool InputManager::keyUp[Key::KeyCount] = {};
bool InputManager::keyDown[Key::KeyCount] = {};

Vec2 InputManager::mousePosition = Vec2(0, 0);
bool InputManager::buttonPressed[Button::ButtonCount] = {};
bool InputManager::buttonUp[Button::ButtonCount] = {};
bool InputManager::buttonDown[Button::ButtonCount] = {};

int InputManager::scrollWheelDirection = 0;

#pragma region Runtime
void InputManager::update(const sf::Event pEvent) {
	sf::Vector2i mp = sf::Mouse::getPosition(*Game::window);
	mousePosition = Vec2(mp.x, mp.y);

	CheckKey(pEvent);
	CheckButton(pEvent);
	UpdateScrollWheel(pEvent);
}

void InputManager::UpdateKeys() {
	for (int i = 0; i < Key::KeyCount; i++) {
		keyDown[i] = false;
		keyUp[i] = false;
	}
}

void InputManager::UpdateMouse() {
	for (int i = 0; i < Button::ButtonCount; i++) {
		buttonDown[i] = false;
		buttonUp[i] = false;
	}
}

void InputManager::UpdateScrollWheel(const sf::Event& pEvent) {

	if (pEvent.type != sf::Event::MouseWheelScrolled)
		return;

	scrollWheelDirection = pEvent.mouseWheelScroll.delta;
}

void InputManager::CheckKey(const sf::Event& pEvent) {
	if (pEvent.type == sf::Event::KeyPressed) {
		keyPressed[pEvent.key.code] = true;
		keyDown[pEvent.key.code] = true;
		return;
	}

	if (pEvent.type == sf::Event::KeyReleased) {
		keyPressed[pEvent.key.code] = false;
		keyUp[pEvent.key.code] = true;
		return;
	}
}

void InputManager::CheckButton(const sf::Event& pEvent) {
	if (pEvent.type == sf::Event::MouseButtonPressed) {
		buttonPressed[pEvent.key.code] = true;
		buttonDown[pEvent.key.code] = true;
		return;
	}

	if (pEvent.type == sf::Event::MouseButtonReleased) {
		buttonPressed[pEvent.key.code] = false;
		buttonUp[pEvent.key.code] = true;
		return;
	}
}
#pragma endregion

#pragma region Keyboard Input

bool InputManager::IsKeyPressed(const Key& pKey) {
	return keyPressed[pKey];
}

bool InputManager::IsKeyUp(const Key& pKey) {
	return keyUp[pKey];
}

bool InputManager::IsKeyDown(const Key& pKey) {
	return keyDown[pKey];
}
#pragma endregion

#pragma region Mouse Input
bool InputManager::IsButtonPressed(const Button& pKey) {
	return buttonPressed[pKey];
}

bool InputManager::IsButtonUp(const Button& pKey) {
	return buttonUp[pKey];
}

bool InputManager::IsButtonDown(const Button& pKey) {
	return buttonDown[pKey];
}
#pragma endregion

#pragma region Scroll Wheel
int InputManager::ReturnScrollDirection() {
	return scrollWheelDirection;
}
#pragma endregion

#pragma region Helper Methods
InputManager* InputManager::GetInstance() {
	if (instance == nullptr)
		instance = new InputManager();
	return instance;
}

void InputManager::StartUpdate() {
	UpdateKeys();
	UpdateMouse();
	scrollWheelDirection = 0;
}
#pragma endregion