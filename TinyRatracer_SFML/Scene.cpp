#include "Scene.h"

void Scene::AddSphere(const Sphere & sphere)
{
	objects.emplace_back(sphere);
}

void Scene::AddLight(const Light & light)
{
	lights.emplace_back(light);
}
