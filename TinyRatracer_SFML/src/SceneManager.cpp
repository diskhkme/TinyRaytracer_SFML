#include "SceneManager.h"

SceneManager::SceneManager()
	:objects{}, lights{}
{
}

void SceneManager::AddSphere(const Sphere & sphere)
{
	objects.emplace_back(sphere);
}

void SceneManager::AddLight(const Light & light)
{
	lights.emplace_back(light);
}

bool SceneManager::EditScene()
{
	bool isEdited = false;
	for (size_t i = 0; i < objects.size(); i++)
	{
		std::string label = "Sphere" + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			isEdited |= objects[i].EditSphere();
			ImGui::TreePop();
		}
	}
	ImGui::Separator();
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string label = "Light" + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			isEdited |= lights[i].EditLight();
			ImGui::TreePop();
		}
	}

	return isEdited;
}


