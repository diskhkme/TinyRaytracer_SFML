#pragma once

#include <vector>

#include "Sphere.h"
#include "Light.h"

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

#include "SFML/Graphics/Image.hpp"

class SceneManager
{
private:
	std::vector<Sphere> objects;
	std::vector<Light> lights;
	sf::Image envMapImg;
public:
	SceneManager();
	void AddSphere(const Sphere& sphere);
	void AddLight(const Light& light);
	void AddEnvironmentMap(const std::string& imgFilename);
	Vec3f GetEnvironmentColor(const Vec3f& direction) const;

	bool EditScene();

	inline const std::vector<Sphere>& GetObjects() const { return objects; }
	inline const std::vector<Light>& GetLights() const { return lights; }
};