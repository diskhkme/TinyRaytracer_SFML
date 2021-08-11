#include "Scene.h"

#include <exception>
#include <cmath>

Scene::~Scene()
{
	for (ModelBase* mb : objects)
	{
		delete mb;
	}

	for (Light* l : lights)
	{
		delete l;
	}
}

void Scene::AddSphere(const Sphere & sphere)
{
	objects.emplace_back(new Sphere{ sphere });
}

void Scene::AddObjModel(const char* filename, const Material& m)
{
	objects.emplace_back(new Model{ filename, m });
}

void Scene::AddLight(const Light& light)
{
	lights.emplace_back(new Light{ light });
}

void Scene::AddEnvironmentMap(const std::string & imgFilename)
{
	if (!envMapImg.loadFromFile(imgFilename))
	{
		std::cout << "Image loading failed: " << imgFilename << std::endl;
		throw;
	}
}

Vec3f Scene::GetEnvironmentColor(const Vec3f & direction) const
{
	float theta = acos(direction.y);
	float phi = atan2(direction.z , direction.x);
	
	unsigned int mapSizeX = envMapImg.getSize().x;
	unsigned int mapSizeY = envMapImg.getSize().y;

	// [-pi:pi] --> [0:1] --> [0:mapsizeY]
	unsigned int pixelXCoord = static_cast<unsigned int>((((phi / PI) + 1.0f) / 2.0f) * mapSizeX); 
	// [0:pi] --> [0:1] --> [0:mapsizeX]
	unsigned int pixelYCoord = static_cast<unsigned int>(((theta / PI)) * mapSizeY); 

	sf::Color c = envMapImg.getPixel(pixelXCoord, pixelYCoord);
	return Vec3f{ c.r / 255.0f, c.g / 255.0f, c.b / 255.0f };
}
