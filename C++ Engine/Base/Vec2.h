#pragma once
#include <iostream>
#include "SFML/System/Vector2.hpp";

class Vec2 {

public:
	float x;
	float y;
public: 

#pragma region Constructor/Destructor
	Vec2();
	Vec2(float pX,float pY);
	~Vec2();
#pragma endregion

#pragma region Operator Overload
	Vec2 operator+(Vec2& pOther);
	Vec2 operator+=(Vec2& pOther);
	Vec2 operator-(Vec2& pOther);
	Vec2 operator-=(Vec2& pOther);
	Vec2 operator*(float pOther);
	Vec2 operator*=(float pOther);
	Vec2 operator/(float pOther);
	Vec2 operator/=(float pOther);
	friend std::ostream& operator<<(std::ostream& os, const Vec2& dt);
#pragma endregion

#pragma region Basic Operations
	void SetXY(const float pX,const float pY);
	float Length()const;
	void Normalize();
	Vec2 Normalized()const;
#pragma endregion

#pragma region Angle Conversion
	static float DegToRad(const float pAngle);
	static float RadToDeg(const float pAngle);
#pragma endregion

#pragma region Angle Operations
	void SetAngleRadians(const float pAngle);
	void SetAngleDegrees(const float pAngle);
	void RotateRadians(const float pAngle);
	void RotateDegrees(const float pAngle);
	Vec2 GetUnitVectorRadians(const float pAngle);
	Vec2 GetUnitVectorDegrees(const float pAngle);
	float GetAngleDegrees()const;
	float GetAngleRadians()const;
	void RotateAroundDegrees(Vec2& pPos,const float pAngle);
	void RotateAroundRadians(Vec2& pPos, const float pAngle);
	void RotateTowardsDegrees(const Vec2& pTarget,float pMaxAngle=180);
	void Reflect(const float pBounciness, const Vec2& pCollisionNormal);

#pragma endregion

#pragma region Special Operations
	float Dot(const Vec2& pOther)const;
	Vec2 Normal()const;
	Vec2 ReversedNormal()const;
	static sf::Vector2f Vec2ToVector2f(const Vec2& pOther);
	static Vec2 Vector2fToVec2(const sf::Vector2f& pOther);
#pragma endregion

};

//c++ is fucking weird man
Vec2 operator* (float x, const Vec2& pOther);
Vec2 operator* (const Vec2& pOther, float x);
Vec2 operator- (const Vec2& pLeft, const Vec2& pRight);
Vec2 operator+ (const Vec2& pLeft, const Vec2& pRight);
Vec2 operator+= (Vec2& pLeft, const Vec2& pRight);
Vec2 operator-= (Vec2& pLeft, const Vec2& pRight);