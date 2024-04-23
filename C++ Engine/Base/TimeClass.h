#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>

class TimeClass {
private:
	sf::Clock clock;
	sf::Time elapsedTime;
public:
	static TimeClass* instance;
	static float time;
	static float deltaTime;
protected:

#pragma region Constructor/Destructor
	TimeClass();
	virtual ~TimeClass();
#pragma endregion

public:

#pragma region Deleted Methods
	TimeClass(TimeClass& pOther) = delete;
	void operator=(const TimeClass&) = delete;
#pragma endregion

#pragma region Instance
	static TimeClass* GetInstance();
#pragma endregion

#pragma region Runtime
	void update();
#pragma endregion
};