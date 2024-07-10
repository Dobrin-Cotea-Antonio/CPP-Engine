#include "RectangleCollider.h"
#include "../Base/GameObject.h"
#include "../Base/Transform.h"
#include "../Physics/CircleCollider.h"
#include <cmath>

#pragma region Constructor/Destructor
RectangleCollider::RectangleCollider() {}

RectangleCollider::~RectangleCollider() {}
#pragma endregion

#pragma region Collision Detection
CollisionInfo RectangleCollider::GetEarliestCollision(const std::weak_ptr<Collider> pOther, const Vec2 pVelocity) {
	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return GetEarliestCollision(circle.get(), pVelocity);

	std::shared_ptr<RectangleCollider> rectangle = std::dynamic_pointer_cast<RectangleCollider>(pOther.lock());

	if (rectangle.get() != nullptr)
		return GetEarliestCollision(rectangle.get(), pVelocity);

	return CollisionInfo();
}

bool RectangleCollider::Overlaps(std::weak_ptr<Collider> pOther) {

	std::shared_ptr<CircleCollider> circle = std::dynamic_pointer_cast<CircleCollider>(pOther.lock());

	if (circle.get() != nullptr)
		return Overlaps(circle.get());

	std::shared_ptr<RectangleCollider> rectangle = std::dynamic_pointer_cast<RectangleCollider>(pOther.lock());

	if (rectangle.get() != nullptr)
		return Overlaps(rectangle.get());

	return false;
}

CollisionInfo RectangleCollider::GetEarliestCollision(RectangleCollider* pOther, const Vec2 pVelocity) {
	float minOverlap = FLT_MAX;
	Vec2 moveBackDist;

	std::vector<Vec2> normalAxis;

	ReturnNormalsAxis(normalAxis, this, pOther);

	for (int i = 0; i < normalAxis.size(); i++) {
		Vec2 axis = normalAxis[i];

		Vec2 projection1 = ProjectOntoAxis(this, axis);
		Vec2 projection2 = ProjectOntoAxis(pOther, axis);

		float overlap = GetRectOverlap(projection1, projection2);

		if (!overlap)
			return CollisionInfo();
		else
			if (overlap < minOverlap) {
				minOverlap = overlap;
				moveBackDist = axis * overlap;
			}
	}

	if ((this->ReturnGlobalCenter() - pOther->ReturnGlobalCenter()).Dot(moveBackDist) < 0.0f)
		moveBackDist *= -1;

	owner.lock()->transform.lock()->localPosition += moveBackDist;

	return CollisionInfo();
}

CollisionInfo RectangleCollider::GetEarliestCollision(CircleCollider* pOther, const Vec2 pVelocity) {
	float minOverlap = FLT_MAX;
	Vec2 moveBackDist;

	std::vector<Vec2> normalAxis;

	ReturnNormalsAxis(normalAxis, this,nullptr);
	normalAxis.push_back(ReturnCircleAxis(pOther, this));

	for (int i = 0; i < normalAxis.size(); i++) {
		Vec2 axis = normalAxis[i];

		Vec2 circleProjection = ProjectCircleOntoAxis(pOther, axis);
		Vec2 rectProjection = ProjectOntoAxis(this, axis);

		float overlap = GetRectOverlap(circleProjection,rectProjection);
	
		if (!overlap)
			return CollisionInfo();
		else
			if (overlap < minOverlap) {
				minOverlap = overlap;
				moveBackDist = axis * overlap;
			}

		if ((this->ReturnGlobalCenter() - pOther->owner.lock()->transform.lock()->GetGlobalPosition()).Dot(moveBackDist) < 0.0f)
			moveBackDist *= -1;

		owner.lock()->transform.lock()->localPosition += moveBackDist;

	}

	return CollisionInfo();
}

bool RectangleCollider::Overlaps(RectangleCollider* pOther) {

	float minOverlap = FLT_MAX;

	std::vector<Vec2> normalAxis;

	ReturnNormalsAxis(normalAxis, this, pOther);

	for (int i = 0; i < normalAxis.size(); i++) {
		Vec2 axis = normalAxis[i];

		Vec2 projection1 = ProjectOntoAxis(this, axis);
		Vec2 projection2 = ProjectOntoAxis(pOther, axis);

		float overlap = GetRectOverlap(projection1, projection2);

		if (!overlap) {
			//std::cout << "False\n";
			return false;
		}
	}

	//std::cout << "True\n";
	return true;
}

bool RectangleCollider::Overlaps(CircleCollider* pOther) {

	float minOverlap = FLT_MAX;
	Vec2 moveBackDist;

	std::vector<Vec2> normalAxis;

	ReturnNormalsAxis(normalAxis, this, nullptr);
	normalAxis.push_back(ReturnCircleAxis(pOther, this));

	for (int i = 0; i < normalAxis.size(); i++) {
		Vec2 axis = normalAxis[i];

		Vec2 circleProjection = ProjectCircleOntoAxis(pOther, axis);
		Vec2 rectProjection = ProjectOntoAxis(this, axis);

		float overlap = GetRectOverlap(circleProjection, rectProjection);

		if (!overlap) {
			//std::cout << "False\n";
			return false;
		}
	}

	//std::cout << "True\n";
	return true;
}

#pragma endregion

#pragma region Helper Methods
void RectangleCollider::GetGlobalBounds() {
	//bounding box with rotation

	Transform* transform = owner.lock()->transform.lock().get();
	Vec2 globalPosition = transform->GetGlobalPosition();
	float globalRotation = transform->GetGlobalRotation();

	for (int i = 0; i < 4; i++) {
		verticesGlobal[i] = vertices[i] + globalPosition;
		verticesGlobal[i].RotateAroundDegrees(globalPosition, globalRotation);
	}
}

void RectangleCollider::ReturnNormalsAxis(std::vector<Vec2>& pNormals, RectangleCollider* pRect1, RectangleCollider* pRect2 = nullptr) {
	pNormals.clear();

	std::vector<Vec2> rectVertices;

	pRect1->ReturnGlobalBounds(rectVertices);

	for (int i = 1; i < rectVertices.size(); i++) {
		Vec2 axis = rectVertices[i] - rectVertices[i - 1];
		axis.Normalize();

		Vec2 normal = axis.Normal();

		bool wasFound = false;

		for (int j = 0; j < pNormals.size(); j++)
			if (pNormals[j] == normal) {// || pNormals[i] == Vec2(-normal.x, normal.y)
				wasFound = true;
				break;
			}

		if (!wasFound)
			pNormals.push_back(normal);

	}

	if (pRect2 == nullptr)
		return;

	pRect2->ReturnGlobalBounds(rectVertices);

	for (int i = 1; i < rectVertices.size(); i++) {
		Vec2 axis = rectVertices[i] - rectVertices[i - 1];
		axis.Normalize();

		Vec2 normal = axis.Normal();

		bool wasFound = false;

		for (int j = 0; j < pNormals.size(); j++)
			if (pNormals[i] == normal || pNormals[i] == Vec2(-normal.x, normal.y)) {
				wasFound = true;
				break;
			}

		if (!wasFound)
			pNormals.push_back(normal);

	}
}

Vec2 RectangleCollider::ReturnCircleAxis(CircleCollider* pCircle, RectangleCollider* pRect) {

	Vec2 circleCenter = pCircle->owner.lock()->transform.lock()->GetGlobalPosition();

	int index = 0;
	float distance = FLT_MAX;

	std::vector<Vec2> rectVertices;

	pRect->ReturnGlobalBounds(rectVertices);

	for (int i = 0; i < rectVertices.size(); i++) {
		float dist = (rectVertices[i] - circleCenter).Length();

		if (dist >= distance)
			continue;

		index = i;
		distance = dist;
	}

	return (circleCenter - rectVertices[index]).Normalized();
}

// returns the min and max projection onto an axis of a rectangle collider
Vec2 RectangleCollider::ProjectOntoAxis(std::vector<Vec2>& pVertices, Vec2 pAxis)const {
	float min = -FLT_MAX;
	float max = FLT_MAX;

	for (int i = 0; i < pVertices.size(); i++) {
		float projection = pVertices[i].Dot(pAxis);

		if (projection < min)
			min = projection;

		if (projection > max)
			max = projection;
	}

	return Vec2(min, max);
}

Vec2 RectangleCollider::ProjectCircleOntoAxis(CircleCollider* pCircle, Vec2 pAxis) const {

	Vec2 dir = pCircle->radius * pAxis;

	Vec2 circleCenter = pCircle->owner.lock()->transform.lock()->GetGlobalPosition();

	Vec2 p1 = circleCenter + dir;
	Vec2 p2 = circleCenter - dir;

	float min = p1.Dot(pAxis);
	float max = p2.Dot(pAxis);

	return Vec2(min, max);
}

float RectangleCollider::GetRectOverlap(const Vec2 pRect1Values, const Vec2 pRect2Values) const {
	return !(pRect1Values.x <= pRect2Values.y && pRect1Values.y >= pRect2Values.x) ? 0.0f : std::min(pRect1Values.y, pRect2Values.y) - std::max(pRect1Values.x, pRect2Values.x);
}

Vec2 RectangleCollider::ProjectOntoAxis(RectangleCollider* pCollider, Vec2 pAxis)const {
	float min = FLT_MAX;
	float max = -FLT_MAX;

	std::vector<Vec2> vertices;

	pCollider->ReturnGlobalBounds(vertices);

	for (int i = 0; i < vertices.size(); i++) {
		float projection = vertices[i].Dot(pAxis);

		if (projection < min)
			min = projection;

		if (projection > max)
			max = projection;
	}

	return Vec2(min, max);
}


void RectangleCollider::SetBounds(const float pWidth, const float pHeight) {
	//bounding box without rotation

	Transform* transform = owner.lock()->transform.lock().get();

	vertices[0] = Vec2(-pWidth / 2, -pHeight / 2);
	vertices[1] = Vec2(pWidth / 2, -pHeight / 2);
	vertices[2] = Vec2(pWidth / 2, pHeight / 2);
	vertices[3] = Vec2(-pWidth / 2, pHeight / 2);
}

void RectangleCollider::ReturnGlobalBounds(std::vector<Vec2>& pVerticesGlobal) {
	pVerticesGlobal.clear();

	GetGlobalBounds();

	for (int i = 0; i < 4; i++)
		pVerticesGlobal.push_back(verticesGlobal[i]);
}

Vec2 RectangleCollider::ReturnGlobalCenter() const {
	return owner.lock()->transform.lock()->GetGlobalPosition();
}
#pragma endregion
