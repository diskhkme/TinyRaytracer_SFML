#include "Window.h"
#include "Scene.h"

int main()
{
	constexpr unsigned int WIDTH = 800;
	constexpr unsigned int HEIGHT = 600;
	constexpr float FOV = 3.14f / 3.0f;

	const Material shinyIvoryMaterial{ Vec2f{0.6f, 0.3f}, Vec3f{0.4f,0.4f,0.3f}, 50.0f };
	const Material dullRedMaterial{ Vec2f{0.7f, 0.1f}, Vec3f{0.5f,0.1f,0.1f}, 5.0f };

	Window window{ WIDTH, HEIGHT, FOV };

	Scene scene;
	// Add sphere objects in scene
	scene.AddSphere({ Vec3f(-3.0f, 0.0f, -16.0f), 2.0f, shinyIvoryMaterial });
	scene.AddSphere({ Vec3f(-1.0f,-1.5f, -12.0f), 2.0f, dullRedMaterial });
	scene.AddSphere({ Vec3f( 1.5f,-0.5f, -18.0f), 3.0f, dullRedMaterial });
	scene.AddSphere({ Vec3f( 7.0f, 5.0f, -18.0f), 4.0f, shinyIvoryMaterial });
	   
	// Add lights in scene
	scene.AddLight({ Vec3f(-20.0f, 20.0f, 20.0f), 1.5f });
	scene.AddLight({ Vec3f( 30.0f, 50.0f,-25.0f), 1.8f });
	scene.AddLight({ Vec3f( 30.0f, 20.0f, 30.0f), 1.7f });

	window.SetScene(scene);
	window.Run();
	   
	return 0;
}