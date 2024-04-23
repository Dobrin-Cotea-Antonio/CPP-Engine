#include "AnimatedSpriteRenderer.h"
#include "TimeClass.h"
#include "TextureCache.h"
#include <SFML/Graphics.hpp>;

#pragma region Constructor/Destructor
AnimatedSpriteRenderer::AnimatedSpriteRenderer() :SpriteRenderer() {
}
AnimatedSpriteRenderer::~AnimatedSpriteRenderer() {
}
#pragma endregion

#pragma region Sprite
void AnimatedSpriteRenderer::LoadSprite(const std::string& pTextureAdress) {
	if (sprite.getTexture() != nullptr)//might need to check for NULL
		TextureCache::GetInstance()->DecreaseTextureCount(textureAdress);

	textureAdress = pTextureAdress;
	sf::Texture& t = TextureCache::GetInstance()->GetTexture(pTextureAdress);
	sprite.setTexture(t);

	frameSize.x = sprite.getLocalBounds().width / animData.cols;
	frameSize.y = sprite.getLocalBounds().height / animData.rows;
	SelectSubFrame();

	sprite.setOrigin(frameSize.x/2, frameSize.y/2);
}
#pragma endregion

#pragma region Runtime
void AnimatedSpriteRenderer::update() {
	elapsedTime += TimeClass::deltaTime;
	if (animData.animate) 
		Animate();
	

}

#pragma endregion

#pragma region Animation
void AnimatedSpriteRenderer::Animate() {
	if (animate && elapsedTime > animData.animationSpeed) {
		if (!animationIsInReverse) {
			currentFrame++;
			if (currentFrame > animationCycleEnd)
				currentFrame = animationCycleStart;
		}
		else {
			currentFrame--;
			if (currentFrame < animationCycleEnd)
				currentFrame = animationCycleStart;
		}

		SelectSubFrame();
		elapsedTime = 0;
	}
}

void AnimatedSpriteRenderer::SelectSubFrame() {
	int left = frameSize.x * (currentFrame % animData.cols);
	int top = frameSize.y * (currentFrame / animData.cols);
	sprite.setOrigin(frameSize.x / 2, frameSize.y / 2);
	sf::IntRect rect(left, top, frameSize.x, frameSize.y);
	sprite.setTextureRect(rect);
}

void AnimatedSpriteRenderer::SetAnimationCycle(int pFrameStart, int pFrameEnd) {
	if (pFrameStart < 0)
		pFrameStart = 0;

	if (pFrameStart > animData.frames - 1)
		pFrameStart = animData.frames - 1;

	if (pFrameEnd < 0)
		pFrameEnd = 0;

	if (pFrameEnd > animData.frames - 1)
		pFrameEnd = animData.frames - 1;

	animationCycleStart = pFrameStart;
	animationCycleEnd = pFrameEnd;

	if (pFrameStart > pFrameEnd)
		animationIsInReverse = true;
	else
		animationIsInReverse = false;

	currentFrame = pFrameStart;
	SelectSubFrame();
}

void AnimatedSpriteRenderer::ChangeAnimationState(const bool pState) {
	animate = pState;
	elapsedTime = 0;
}

void AnimatedSpriteRenderer::SetAnimationData(const AnimationData& pAnimationData) {
	animData = pAnimationData;
	LoadSprite(animData.imageAdress);
	SetAnimationCycle(0,animData.frames-1);
}
#pragma endregion