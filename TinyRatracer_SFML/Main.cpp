#include <cassert>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Geometry.h"
#include "Renderer.h"
#include "Sphere.h"

inline sf::Uint8 ConvertFloatToColor(float val)
{
	return (val >= 0.0f) ? static_cast<sf::Uint8>(val*255.0f) : 0;
}

void ConvertPixelsFromVector(const std::vector<Vec3f>& framebuffer, sf::Uint8* pixels)
{
	size_t pixelCount = framebuffer.size();

	for (size_t i = 0; i < pixelCount; i++)
	{
		pixels[4 * i + 0] = ConvertFloatToColor(framebuffer[i].x);
		pixels[4 * i + 1] = ConvertFloatToColor(framebuffer[i].y);
		pixels[4 * i + 2] = ConvertFloatToColor(framebuffer[i].z);
		pixels[4 * i + 3] = 255;
	}
}

int main()
{
	constexpr unsigned int WIDTH = 800;
	constexpr unsigned int HEIGHT = 600;
	constexpr float FOV = 3.14f / 2.f;
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Tiny Ray tracer");
	window.setVerticalSyncEnabled(true);

	sf::Texture targetTexture;
	if (!targetTexture.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	std::vector<Vec3f> framebuffer(WIDTH * HEIGHT);
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	
	Renderer renderer = Renderer{ WIDTH, HEIGHT, FOV };

	//---Scene 정의
	Sphere sphere{ Vec3f(0,0,-5),1.f };

	//---Render
	sf::Clock clock;
	renderer.Render(framebuffer, sphere);
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	window.setTitle(sf::String{ std::to_string(elapsedTime) + " ms" });

	//---Texture Update
	ConvertPixelsFromVector(framebuffer, pixels);
	targetTexture.update(pixels);

	sf::Sprite sprite;
	sprite.setTexture(targetTexture);

	//---Display
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	delete pixels;
	return 0;
}