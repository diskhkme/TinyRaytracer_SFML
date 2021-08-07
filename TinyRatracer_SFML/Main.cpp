#include <cassert>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Geometry.h"

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
	constexpr int WIDTH = 800;
	constexpr int HEIGHT = 600;
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
	window.setVerticalSyncEnabled(true);

	sf::Texture targetTexture;
	if (!targetTexture.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	std::vector<Vec3f> framebuffer(WIDTH * HEIGHT);
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	sf::Clock clock;
	for (size_t j = 0; j < HEIGHT; j++)
	{
		for (size_t i = 0; i < WIDTH; i++)
		{
			framebuffer[i + j * WIDTH] = Vec3f(j / float(HEIGHT), i / float(WIDTH), 0);
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	window.setTitle(sf::String{ std::to_string(elapsedTime) + " ms" });


	ConvertPixelsFromVector(framebuffer, pixels);
	targetTexture.update(pixels);

	sf::Sprite sprite;
	sprite.setTexture(targetTexture);

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