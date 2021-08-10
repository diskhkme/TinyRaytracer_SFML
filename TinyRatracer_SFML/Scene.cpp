#include "Scene.h"

#include <exception>

void Scene::AddSphere(const Sphere & sphere)
{
	objects.emplace_back(sphere);
}

void Scene::AddLight(const Light & light)
{
	lights.emplace_back(light);
}

void Scene::AddEnvironmentMap(const std::string & imgFilename)
{
	if (!envMapImg.loadFromFile(imgFilename))
	{
		std::cout << "Image loading failed: " << imgFilename << std::endl;
		throw;
	}
}
