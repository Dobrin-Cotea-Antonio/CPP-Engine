#pragma once
#include "../Base/Vec2.h"

class Collider;

struct CollisionInfo {

	Vec2 normal;
	Collider* other;
	float timeOfImpact;

	CollisionInfo(const Vec2 pNormal, Collider* pOther, const float pTimeOfImpact);
	CollisionInfo();
};