#include "Window.h"

int main()
{
	constexpr unsigned int WIDTH = 800;
	constexpr unsigned int HEIGHT = 600;
	constexpr float FOV = 3.14f / 3.0f;

	const Vec3f ivoryColor{ 0.4f,0.4f,0.3f };
	const Vec3f redColor{ 0.3f,0.1f,0.1f };

	Window window{ WIDTH, HEIGHT, FOV };

	window.AddSphere(Sphere{ Vec3f(-3.0f, 0.0f, -16.0f), 2.0f, ivoryColor });
	window.AddSphere(Sphere{ Vec3f(-1.0f,-1.5f, -12.0f), 2.0f, redColor });
	window.AddSphere(Sphere{ Vec3f( 1.5f,-0.5f, -18.0f), 3.0f, redColor });
	window.AddSphere(Sphere{ Vec3f( 7.0f, 5.0f, -18.0f), 4.0f, ivoryColor });

	window.Run();
	   
	return 0;
}