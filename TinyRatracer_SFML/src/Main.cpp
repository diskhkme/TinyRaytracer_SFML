#include "Window.h"

int main()
{
	constexpr unsigned int WIDTH = 1024;
	constexpr unsigned int HEIGHT = 768;
	constexpr unsigned int PREVIEW_WIDTH = 64;
	constexpr unsigned int PREVIEW_HEIGHT = 48;
	constexpr float FOV = 3.14f / 3.f;
	size_t MAXDEPTH = 4;

	const Material shinyIvoryMaterial{ Vec4f{0.6f, 0.3f, 0.1f, 0.0f }, Vec3f{0.4f,0.4f,0.3f}, 50.0f, 1.0f };
	const Material dullRedMaterial{ Vec4f{0.7f, 0.1f, 0.0f, 0.0f }, Vec3f{0.5f,0.1f,0.1f}, 5.0f, 1.0f };
	const Material mirrorMaterial{ Vec4f{0.0f, 10.0f, 0.8f, 0.0f }, Vec3f{1.0f,1.0f,1.0f}, 1425.0f, 1.0f };
	const Material glassMaterial{ Vec4f{0.0f, 0.5f, 0.1f, 0.8f }, Vec3f{0.6f,0.7f,0.8f}, 125.0f, 1.5f };
	
	Window* window = new Window{ WIDTH, HEIGHT, FOV, MAXDEPTH, PREVIEW_WIDTH, PREVIEW_HEIGHT };

	SceneManager* scene = new SceneManager;

	// Add sphere objects in scene
	scene->AddSphere({ Vec3f(-3.0f, 0.0f, 0.0f), 2.0f, shinyIvoryMaterial });
	scene->AddSphere({ Vec3f(-1.0f,-1.5f, 4.0f), 2.0f, glassMaterial });
	scene->AddSphere({ Vec3f(1.5f,-0.5f, -2.0f), 3.0f, dullRedMaterial });
	scene->AddSphere({ Vec3f(7.0f, 5.0f, -2.0f), 4.0f, mirrorMaterial });

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

	delete scene;
	delete window;

	return 0;
}