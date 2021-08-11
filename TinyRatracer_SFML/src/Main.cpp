#include "Window.h"
#include "scene/Scene.h"

#include <exception>

int main()
{
	constexpr unsigned int WIDTH = 1024;
	constexpr unsigned int HEIGHT = 768;
	constexpr float FOV = PI / 3.0f;
	constexpr size_t MAXDEPTH = 4;

	const Material shinyIvoryMaterial{ Vec4f{0.6f, 0.3f, 0.1f, 0.0f }, Vec3f{0.4f,0.4f,0.3f}, 50.0f, 1.0f };
	const Material dullRedMaterial{ Vec4f{0.9f, 0.1f, 0.0f, 0.0f }, Vec3f{0.3f,0.1f,0.1f}, 10.0f, 1.0f };
	const Material mirrorMaterial{ Vec4f{0.0f, 10.0f, 0.8f, 0.0f }, Vec3f{1.0f,1.0f,1.0f}, 1425.0f, 1.0f };
	const Material glassMaterial{ Vec4f{0.0f, 0.5f, 0.1f, 0.8f }, Vec3f{0.6f,0.7f,0.8f}, 125.0f, 1.5f };
	
	Window* window = new Window{ WIDTH, HEIGHT, FOV, MAXDEPTH };

	Scene* scene = new Scene;
	// Add sphere objects in scene
	scene->AddSphere({ Vec3f(-3.0f, 0.0f, -16.0f), 2.0f, shinyIvoryMaterial });
	scene->AddSphere({ Vec3f(-1.0f,-1.5f, -12.0f), 2.0f, glassMaterial });
	scene->AddSphere({ Vec3f(1.5f,-0.5f, -18.0f), 3.0f, dullRedMaterial });
	scene->AddSphere({ Vec3f(7.0f, 5.0f, -18.0f), 4.0f, mirrorMaterial });

	// Add .obj model in scene
	scene->AddObjModel("resources/duck.obj", glassMaterial);

	// Add lights in scene
	scene->AddLight({ Vec3f(-20.0f, 20.0f, 20.0f), 1.5f });
	scene->AddLight({ Vec3f(30.0f, 50.0f,-25.0f), 1.8f });
	scene->AddLight({ Vec3f(30.0f, 20.0f, 30.0f), 1.7f });

	// Add environment
	try
	{
		scene->AddEnvironmentMap("resources/envmap.jpg");
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		delete window;
		return -1;
	}

	window->SetScene(scene);
	window->Run();

	return 0;
}