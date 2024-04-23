#pragma once
#include "Component.h"
#include<SFML/Graphics.hpp>

class SpriteRenderer : public Component {
protected:
	sf::Sprite sprite;
	sf::RenderWindow* window = nullptr;
	std::string textureAdress;

public:
#pragma region Constructor/Destructor
	SpriteRenderer();
	virtual ~SpriteRenderer();
#pragma endregion

#pragma region Runtime
	virtual void render()override;
#pragma endregion

#pragma region Sprite
	virtual void LoadSprite(const std::string& pTextureAdress);
	virtual void Draw();
#pragma endregion

};