#pragma once

#include "Material.h"

class ModelBase
{
private:
	Material material;

public:
	ModelBase(const Material& m) : material{ m } {}
	virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir, 
		float& closest, Vec3f& hit, Vec3f& normal) const = 0;

public:
	inline const Material& GetMaterial() const {return material;}
};
