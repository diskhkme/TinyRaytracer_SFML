#pragma once

#include <vector>

#include "Sphere.h"
#include "Light.h"

class Scene
{
private:
	std::vector<Sphere> objects;
	std::vector<Light> lights;
public:
	Scene() = default;
	void AddSphere(const Sphere& sphere);
	void AddLight(const Light& light);

	inline const std::vector<Sphere>& GetObjects() const { return objects; }
	inline const std::vector<Light>& GetLights() const { return lights; }
};