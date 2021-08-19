#pragma once

#include "../Geometry.h"
#include "RenderOption.h"

class Ray {
public:
	Vec3f origin;
	Vec3f direction;

public:
	Ray() {}
	Ray(const Vec3f& origin, const Vec3f& direction)
		: origin{ origin }, direction{ direction }
	{}

	Vec3f At(dist_t t) const {
		return origin + t * direction;
	}
};
