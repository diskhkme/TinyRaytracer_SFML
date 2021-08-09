#pragma once

#include "Geometry.h"

class Light
{
private:
	Vec3f position;
	float intensity;

public:
	//point light
	Light(const Vec3f& p, const float& i) 
		: position{ p }, intensity{ i }
	{

	}

	inline Vec3f GetPosition() const { return position; }
	inline float GetIntensity() const { return intensity; }
};