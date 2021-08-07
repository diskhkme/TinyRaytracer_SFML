#include "Window.h"

int main()
{
	constexpr unsigned int WIDTH = 400;
	constexpr unsigned int HEIGHT = 300;
	constexpr float FOV = 3.14f / 3.0f;

	Window window{ WIDTH, HEIGHT, FOV };
	window.Run();



	return 0;
}