#pragma once

#include "Geometry.h"
#include "Material.h"

class Sphere
{
private:
	Vec3f center;
	float radius;
	Vec3f color;
	Material material;

public:
	Sphere(const Vec3f& c, const float& r, const Material& m);
	
	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& closest) const;
	
public:
	inline Vec3f GetCenter() const { return center; }
	inline Material GetMaterial() const { return material; }


};