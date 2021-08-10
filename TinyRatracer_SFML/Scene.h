#pragma once

#include <vector>
#include "SFML/Graphics/Image.hpp"

#include "Sphere.h"
#include "Light.h"

class Scene
{
private:
	std::vector<Sphere> objects;
	std::vector<Light> lights;
	sf::Image envMapImg;
public:
	Scene() = default;
	void AddSphere(const Sphere& sphere);
	void AddLight(const Light& light);
	void AddEnvironmentMap(const std::string& imgFilename);
	Vec3f GetEnvironmentColor(const Vec3f& direction) const;

	inline const std::vector<Sphere>& GetObjects() const { return objects; }
	inline const std::vector<Light>& GetLights() const { return lights; }
};