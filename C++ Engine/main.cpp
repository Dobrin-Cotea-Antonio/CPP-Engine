#include<iostream>
#include<SFML/Graphics.hpp>
#include<memory>
#include "Base/GameObject.h"
#include "Base/Vec2.h";
#include "Base/TimeClass.h";
#include "Base/Game.h";
#include "Game/MyGame.h";	
int main() {
	MyGame game(Vec2(640,480));
	game.run();
	return 0;
}


