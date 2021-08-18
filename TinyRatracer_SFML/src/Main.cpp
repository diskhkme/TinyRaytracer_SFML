#include <cassert>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

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

sf::Int32 Render(std::vector<Vec3f>& framebuffer, const int& width, const int& height)
{
	sf::Clock clock;
	for (size_t j = 0; j < height; j++)
	{
		for (size_t i = 0; i < width; i++)
		{
			framebuffer[i + j * width] = Vec3f(j / float(height), i / float(width), 0);
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

sf::Int32 UpdateDisplay(sf::Texture& targetTexture, const std::vector<Vec3f>& framebuffer, sf::Uint8* const pixels)
{
	sf::Clock clock;
	ConvertPixelsFromVector(framebuffer, pixels);
	targetTexture.update(pixels);
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

int main()
{
	constexpr int WIDTH = 1024;
	constexpr int HEIGHT = 768;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TinyRenderer");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Texture targetTexture;
	if (!targetTexture.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	std::vector<Vec3f> framebuffer(WIDTH * HEIGHT);
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	sf::Sprite sprite;
	sprite.setTexture(targetTexture);

	sf::Int32 renderTime = 0;
	sf::Int32 updateTime = 0;

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Menu"); 

		ImGui::Text("Press Render button to render");
		if (ImGui::Button("Render")) {
			renderTime = Render(framebuffer, WIDTH, HEIGHT);
			updateTime = UpdateDisplay(targetTexture, framebuffer, pixels);
		}
		ImGui::Text("Rendering: %d ms \t Update: %d ms", renderTime, updateTime);

		ImGui::End(); // end window

		window.clear();
		window.draw(sprite);
		ImGui::SFML::Render(window);

		window.display();
	}

	return 0;
}