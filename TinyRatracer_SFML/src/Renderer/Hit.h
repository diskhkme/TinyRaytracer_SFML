#pragma once

#include "Ray.h"
#include "../Scene/Material.h"

struct Hit
{
	Vec3f point;
	Vec3f normal;
	Material material;
	dist_t t = std::numeric_limits<dist_t>::max();
};