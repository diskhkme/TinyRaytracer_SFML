#include "Window.h"


int main()
{
	constexpr unsigned int WIDTH = 1024;
	constexpr unsigned int HEIGHT = 768;
	constexpr unsigned int PREVIEW_WIDTH = 128;
	constexpr unsigned int PREVIEW_HEIGHT = 96;
	constexpr float FOV = 3.14f / 2.f;

	Window window{ WIDTH, HEIGHT, FOV, PREVIEW_WIDTH, PREVIEW_HEIGHT };
	window.Run();

	



	

	return 0;
}