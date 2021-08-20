#include "SceneManager.h"
#include "../Utility.h"

SceneManager::SceneManager()
	:objects{}, lights{}
{
}

SceneManager::~SceneManager()
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

void SceneManager::AddSphere(const Sphere & sphere)
{
	objects.emplace_back(new Sphere{ sphere });
}

void SceneManager::AddObjModel(const std::string& name, const char* filepath, const Material& m)
{
	objects.emplace_back(new ObjModel{ name, filepath, m });
}

void SceneManager::AddLight(const Light & light)
{
	lights.emplace_back(new Light{ light });
}

void SceneManager::AddEnvironmentMap(const std::string & imgFilename)
{
	if (!envMapImg.loadFromFile(imgFilename))
	{
		std::cout << "Image loading failed: " << imgFilename << std::endl;
		throw;
	}
}

void SceneManager::SetCamera(const Camera & camera)
{
	this->camera = camera;
}

Camera & SceneManager::GetCamera()
{
	return camera;
}

Vec3f SceneManager::GetEnvironmentColor(const Vec3f & direction) const
{
	float theta = acos(direction.y);
	float phi = atan2(direction.z, direction.x);

	unsigned int mapSizeX = envMapImg.getSize().x;
	unsigned int mapSizeY = envMapImg.getSize().y;

	// [-pi:pi] --> [0:1] --> [0:mapsizeY]
	unsigned int pixelXCoord = static_cast<unsigned int>((((phi / MY_PI) + 1.0f) / 2.0f) * (mapSizeX - 1));
	// [0:pi] --> [0:1] --> [0:mapsizeX]
	unsigned int pixelYCoord = static_cast<unsigned int>(((theta / MY_PI)) * (mapSizeY - 1));

	sf::Color c = envMapImg.getPixel(pixelXCoord, pixelYCoord);
	return Vec3f{ c.r / 255.0f, c.g / 255.0f, c.b / 255.0f };
}

bool SceneManager::EditScene()
{
	bool isEdited = false;
	for (size_t i = 0; i < objects.size(); i++)
	{
		std::string label = objects[i]->GetName();
		if (ImGui::TreeNode(label.c_str()))
		{
			isEdited |= objects[i]->EditModel();
			ImGui::TreePop();
		}
	}
	ImGui::Separator();
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string label = "Light" + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			isEdited |= lights[i]->EditLight();
			ImGui::TreePop();
		}
	}

	return isEdited;
}


