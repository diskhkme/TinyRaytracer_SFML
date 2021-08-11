#pragma once

#include <vector>
#include "SFML/Graphics/Image.hpp"

#include "model/Sphere.h"
#include "model/Model.h"
#include "../Light.h"

class Scene
{
private:
	std::vector<ModelBase*> objects;
	std::vector<Light*> lights;
	sf::Image envMapImg;
public:
	Scene() = default;
	~Scene();
	void AddSphere(const Sphere& sphere);
	void AddObjModel(const char* filename, const Material& m);
	void AddLight(const Light& light);
	void AddEnvironmentMap(const std::string& imgFilename);
	Vec3f GetEnvironmentColor(const Vec3f& direction) const;

	inline const std::vector<ModelBase*>& GetObjects() const { return objects; }
	inline const std::vector<Light*>& GetLights() const { return lights; }
};