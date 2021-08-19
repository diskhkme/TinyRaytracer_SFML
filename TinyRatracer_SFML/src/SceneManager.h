#pragma once

#include <vector>

#include "Sphere.h"
#include "Light.h"

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

class SceneManager
{
private:
	std::vector<Sphere> objects;
	std::vector<Light> lights;
public:
	SceneManager();
	void AddSphere(const Sphere& sphere);
	void AddLight(const Light& light);

	bool EditScene();

	inline const std::vector<Sphere>& GetObjects() const { return objects; }
	inline const std::vector<Light>& GetLights() const { return lights; }
};