#include <cassert>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

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
	constexpr unsigned int WIDTH = 1024;
	constexpr unsigned int HEIGHT = 768;
	constexpr unsigned int PREVIEW_WIDTH = 128;
	constexpr unsigned int PREVIEW_HEIGHT = 96;
	constexpr float FOV = 3.14f / 2.f;

	//--- Window and FrameBuffer 초기화
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TinyRenderer");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	//--- Full resolution 
	sf::Texture targetTexture;
	if (!targetTexture.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	std::vector<Vec3f> framebuffer(WIDTH * HEIGHT);
	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	//--- Preview resolution
	sf::Texture previewTexture;
	if (!previewTexture.create(PREVIEW_WIDTH, PREVIEW_HEIGHT))
	{
		return -1;
	}

	std::vector<Vec3f> preViewframebuffer(PREVIEW_WIDTH * PREVIEW_HEIGHT);
	sf::Uint8* previewPixels = new sf::Uint8[PREVIEW_WIDTH * PREVIEW_HEIGHT * 4];

	sf::Sprite sprite;
	sprite.setTexture(previewTexture, true);

	//---Renderer 초기화
	Renderer renderer = Renderer{ WIDTH, HEIGHT, FOV, PREVIEW_WIDTH, PREVIEW_HEIGHT};

	//---Scene 정의
	Sphere sphere{ Vec3f{0,0,-5},1.f, Vec3f{0.4f, 0.4f, 0.3f} };

	
	sf::Int32 renderTime = 0;
	sf::Int32 updateTime = 0;
	float sphereColor[3] = { 0.4f, 0.4f, 0.3f };
	bool isRenderedCurrently = false;

	//---Window 그리기
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

		//Preview Render
		if (!isRenderedCurrently)
		{
			renderer.Render(preViewframebuffer, sphere, true);
			UpdateDisplay(previewTexture, preViewframebuffer, previewPixels);
			sprite.setTexture(previewTexture, true);
			sprite.setScale((float)WIDTH / PREVIEW_WIDTH, (float)HEIGHT / PREVIEW_HEIGHT);
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		//---Render Menu
		ImGui::Begin("Menu"); 
		ImGui::Text("Press Render button to render");
		if (ImGui::Button("Render")) {
			renderTime = renderer.Render(framebuffer, sphere, false);
			updateTime = UpdateDisplay(targetTexture, framebuffer, pixels);
			sprite.setTexture(targetTexture, true);
			sprite.setScale(1.0f, 1.0f);
			isRenderedCurrently = true;
		}
		ImGui::Text("Rendering: %d ms", renderTime);
		ImGui::Text("Update Display: %d ms", updateTime);
		ImGui::End();

		//---Geometry Menu
		ImGui::Begin("Sphere Geometry");
		ImGui::Text("If values changed, preview screen will be shown");
		ImGui::Text("After changing the values, press Render button again");
		isRenderedCurrently = !ImGui::ColorEdit3("Sphere Color", sphereColor);
		sphere.SetColor(sphereColor);
		ImGui::End();

		window.clear();
		window.draw(sprite);
		ImGui::SFML::Render(window);

		window.display();
	}

	delete[] pixels;
	delete[] previewPixels;

	return 0;
}