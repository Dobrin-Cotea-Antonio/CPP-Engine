#include "TimeClass.h"

float TimeClass::time;
float TimeClass::deltaTime;
TimeClass* TimeClass::instance = nullptr;

#pragma region Constructor/Destructor
TimeClass::TimeClass() {
	clock.restart();
	elapsedTime = sf::seconds(0);
	time = 0;
	deltaTime = 0;
}

TimeClass::~TimeClass() {
}
#pragma endregion

#pragma region Instance
TimeClass* TimeClass::GetInstance() {
	if (instance == nullptr)
		instance = new TimeClass();
	return instance;
}
#pragma endregion

#pragma region Runtime
void TimeClass::update() {
	elapsedTime = clock.getElapsedTime();
	clock.restart();
	deltaTime = elapsedTime.asSeconds();
	time += deltaTime;
}
#pragma endregion