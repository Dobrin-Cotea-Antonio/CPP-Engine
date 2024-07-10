#define _USE_MATH_DEFINES

#include "Vec2.h"
#include <cmath>

#pragma region Constructor/Destructor
Vec2::Vec2() {
	x = 0;
	y = 0;
}
Vec2::Vec2(float pX, float pY) :x(pX), y(pY) {
}

Vec2::~Vec2() {
}
#pragma endregion

#pragma region Operator Overload
Vec2 Vec2::operator+(Vec2& pOther) {
	return Vec2(x + pOther.x, y + pOther.y);
}

Vec2 Vec2::operator+=(Vec2& pOther) {
	x += pOther.x;
	y += pOther.y;
	return Vec2(x, y);
}

Vec2 Vec2::operator-(Vec2& pOther) {
	return Vec2(x - pOther.x, y - pOther.y);
}

Vec2 Vec2::operator-=(Vec2& pOther) {
	x -= pOther.x;
	y -= pOther.y;
	return Vec2(x, y);
}

Vec2 operator*(float x, const Vec2& pOther) {
	return Vec2(x * pOther.x, x * pOther.y);
}

Vec2 operator*(const Vec2& pOther, float x) {
	return Vec2(x * pOther.x, x * pOther.y);
}

Vec2 operator-(const Vec2& pLeft, const Vec2& pRight) {
	return Vec2(pLeft.x - pRight.x, pLeft.y - pRight.y);
}

Vec2 operator+(const Vec2& pLeft, const Vec2& pRight) {
	return Vec2(pLeft.x + pRight.x, pLeft.y + pRight.y);
}

Vec2 operator+=(Vec2& pLeft, const Vec2& pRight) {
	pLeft.x += pRight.x;
	pLeft.y += pRight.y;
	return pLeft;
}

Vec2 operator-=(Vec2& pLeft, const Vec2& pRight) {
	pLeft.x -= pRight.x;
	pLeft.y -= pRight.y;
	return pLeft;
}

bool operator==(const Vec2& pLeft, const Vec2& pRight) {
	return (pLeft.x == pRight.x && pLeft.y == pRight.y);
}

Vec2 Vec2::operator*(float pOther) {
	return Vec2(x * pOther, y * pOther);
}

Vec2 Vec2::operator*=(float pOther) {
	x *= pOther;
	y *= pOther;
	return Vec2(x, y);
}

Vec2 Vec2::operator/(float pOther) {
	return Vec2(x / pOther, y / pOther);
}

Vec2 Vec2::operator/=(float pOther) {
	x /= pOther;
	y /= pOther;
	return Vec2(x, y);
}

std::ostream& operator<<(std::ostream& os, const Vec2& dt) {
	os << dt.x << " " << dt.y << "\n";
	return os;
}
#pragma endregion

#pragma region Basic Operations
void Vec2::SetXY(const float pX, const float pY) {
	x = pX;
	y = pY;
}

float Vec2::Length() const {
	return (float)(std::sqrt(x * x + y * y));
}

void Vec2::Normalize() {
	float length = Length();
	if (length) {
		x /= length;
		y /= length;
	}
}

Vec2 Vec2::Normalized() const {
	float length = Length();

	if (length)
		return Vec2(x / length, y / length);

	return Vec2(0, 0);
}
#pragma endregion

#pragma region Angle Conversion
float Vec2::DegToRad(const float pAngle) {
	return (pAngle / 180.0f * M_PI);
}

float Vec2::RadToDeg(const float pAngle) {
	return (pAngle / M_PI * 180.0f);
}

#pragma endregion

#pragma region Angle Operations
void Vec2::SetAngleRadians(const float pAngle) {
	Vec2 unit = GetUnitVectorRadians(pAngle);
	unit = unit * Length();
	SetXY(unit.x, unit.y);
}

void Vec2::SetAngleDegrees(const float pAngle) {
	SetAngleRadians(DegToRad(pAngle));
}

void Vec2::RotateRadians(const float pAngle) {
	float oldX = x;
	float oldY = y;
	float sinA = sin(pAngle);
	float cosA = cos(pAngle);
	x = oldX * cosA - oldY * sinA;
	y = oldX * sinA + oldY * cosA;
}

void Vec2::RotateDegrees(const float pAngle) {
	RotateRadians(DegToRad(pAngle));
}

Vec2 Vec2::GetUnitVectorRadians(const float pAngle) {
	return Vec2(cos(pAngle), sin(pAngle));
}

Vec2 Vec2::GetUnitVectorDegrees(const float pAngle) {
	return GetUnitVectorRadians(DegToRad(pAngle));
}

float Vec2::GetAngleDegrees() const {
	return (atan2(y, x) * (180.0f / M_PI));
}

float Vec2::GetAngleRadians() const {
	return (atan2(y, x));
}

void Vec2::RotateAroundDegrees(Vec2& pPos, const float pAngle) {
	Vec2 pos = Vec2(x - pPos.x, y - pPos.y);
	pos.RotateDegrees(pAngle);
	pos += pPos;
	SetXY(pos.x, pos.y);
}

void Vec2::RotateAroundRadians(Vec2& pPos, const float pAngle) {
	Vec2 pos = Vec2(x - pPos.x, y - pPos.y);
	pos.RotateRadians(pAngle);
	pos += pPos;
	SetXY(pos.x, pos.y);
}

void Vec2::RotateTowardsDegrees(const Vec2& pTarget, float pMaxAngle) {
	if (pMaxAngle <= 0)
		pMaxAngle = 180;
	float targetRotation = pTarget.GetAngleDegrees();
	targetRotation = fmod(targetRotation + 360, 360);//make sure the angle is between 0 and 360
	float currentAngle = GetAngleDegrees();
	currentAngle = fmod(currentAngle + 360, 360);
	float oldAngle = currentAngle;

	if (targetRotation < currentAngle) {
		if (abs(currentAngle - targetRotation) >= 180)
			currentAngle += fmin(abs(targetRotation - currentAngle), pMaxAngle);
		else
			currentAngle -= fmin(abs(targetRotation - currentAngle), pMaxAngle);
	}
	else {
		if (abs(currentAngle - targetRotation) >= 180)
			currentAngle -= fmin(abs(targetRotation - currentAngle), pMaxAngle);
		else
			currentAngle += fmin(abs(targetRotation - currentAngle), pMaxAngle);

	}
	RotateDegrees(currentAngle - oldAngle);
}

void Vec2::Reflect(const float pBounciness, const Vec2& pCollisionNormal) {
	Vec2 v = Vec2(x, y);
	v = (v - pCollisionNormal * (1 + pBounciness) * (v.Dot(pCollisionNormal)));
	SetXY(v.x, v.y);
}
#pragma endregion

#pragma region Special Operations
float Vec2::Dot(const Vec2& pOther) const {
	return (x * pOther.x + y * pOther.y);
}

Vec2 Vec2::Normal() const {
	return (Vec2(-y, x).Normalized());
}

Vec2 Vec2::ReversedNormal() const {
	return (Vec2(y, -x).Normalized());
}

sf::Vector2f Vec2::Vec2ToVector2f(const Vec2& pOther) {
	sf::Vector2f vec;
	vec.x = pOther.x;
	vec.y = pOther.y;
	return vec;
}

Vec2 Vec2::Vector2fToVec2(const sf::Vector2f& pOther) {
	return Vec2(pOther.x, pOther.y);
}
#pragma endregion
