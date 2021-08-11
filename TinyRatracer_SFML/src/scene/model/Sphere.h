#pragma once

#include "../../Geometry.h"
#include "ModelBase.h"
#include <iostream>

class Sphere : public ModelBase
{
private:
	Vec3f center;
	float radius;

public:
	Sphere(const Vec3f& c, const float& r, const Material& m);
	
	virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir, 
		float& closest, Vec3f& hit, Vec3f& normal) const override;
	
public:
	inline const Vec3f& GetCenter() const { return center; }
};