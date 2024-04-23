#include "CollisionInfo.h"

CollisionInfo::CollisionInfo(const Vec2 pNormal, Collider* pOther, const float pTimeOfImpact) :normal(pNormal), other(pOther), timeOfImpact(pTimeOfImpact) {
}

CollisionInfo::CollisionInfo() :timeOfImpact(-1) {
}
