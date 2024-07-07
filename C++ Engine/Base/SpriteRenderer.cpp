#include "SpriteRenderer.h"
#include "TextureCache.h"
#include "GameObject.h"
#include "Transform.h"
#include "../Base/Game.h"
#include <iostream>

#pragma region Constructor/Destructor
SpriteRenderer::SpriteRenderer() :Component() {
	window = Game::window;
}
SpriteRenderer::~SpriteRenderer() {
}

#pragma endregion

#pragma region Runtime
void SpriteRenderer::render() {
	Draw();
}
#pragma endregion

#pragma region Sprite
void SpriteRenderer::LoadSprite(const std::string& pTextureAdress) {
	if (sprite.getTexture() != nullptr)//might need to check for NULL
		TextureCache::GetInstance()->DecreaseTextureCount(textureAdress);

	textureAdress = pTextureAdress;
	sf::Texture& t = TextureCache::GetInstance()->GetTexture(pTextureAdress);
	sprite.setTexture(t);
	sprite.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
}

void SpriteRenderer::Draw() {
	if (window == nullptr)
		return;

	if (IsEnabledGlobal()) {
		sprite.setScale(Vec2::Vec2ToVector2f(owner.lock()->transform.lock()->GetGlobalScale()));
		sprite.setPosition(Vec2::Vec2ToVector2f(owner.lock()->transform.lock()->GetGlobalPosition()));
		sprite.setRotation(owner.lock()->transform.lock()->GetGlobalRotation());
		window->draw(sprite);
	}
}
#pragma endregion