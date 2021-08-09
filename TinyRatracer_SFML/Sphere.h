#pragma once

#include "Geometry.h"

class Sphere
{
private:
	Vec3f center;
	float radius;
	Vec3f color;

public:
	Sphere(const Vec3f& c, const float& r, const Vec3f& color);
	
	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& closest) const;
	
public:
	inline Vec3f GetColor() const { return color; }
	inline Vec3f GetCenter() const { return center; }


};