#pragma once
#include "SpriteRenderer.h"
#include "Vec2.h"
#include "AnimationData.h";

class AnimatedSpriteRenderer :public SpriteRenderer {

protected:
	bool animate = true;
	bool animationIsInReverse = false;
	int currentFrame = 0;
	AnimationData animData;

	int animationCycleStart;
	int animationCycleEnd;

	Vec2 frameSize = Vec2(32, 32);
	float elapsedTime = 0;


#pragma region Constructor/Destructor
public:
	AnimatedSpriteRenderer();
	virtual ~AnimatedSpriteRenderer();
#pragma endregion

#pragma region Sprite
private:
	void LoadSprite(const std::string& pTextureAdress)override;
#pragma endregion

#pragma region Runtime
	void update()override;
#pragma endregion

#pragma region Animation
protected:
	void Animate();
	void SelectSubFrame();
public:
	void SetAnimationCycle(int pFrameStart, int pFrameEnd);
	void SetAnimationData(const AnimationData& pAnimationData);
	void ChangeAnimationState(const bool pState);
#pragma endregion

};