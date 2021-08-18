#include <SFML/Graphics.hpp>

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

int main()
{
	constexpr int WIDTH = 1024;
	constexpr int HEIGHT = 768;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;

	float color[3] = { 0.f, 0.f, 0.f };

	// let's use char array as buffer, see next part
	// for instructions on using std::string with ImGui
	char windowTitle[255] = "ImGui + SFML = <3";


	sf::Texture targetTexture;
	if (!targetTexture.create(WIDTH, HEIGHT))
	{
		return -1;
	}

	sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];

	sf::Clock clock;
	for (size_t j = 0; j < HEIGHT; j++)
	{
		for (size_t i = 0; i < WIDTH; i++)
		{
			pixels[(i + j * WIDTH) * 4 + 0] = static_cast<sf::Uint8>((j / float(HEIGHT)) * 255); // R Channel
			pixels[(i + j * WIDTH) * 4 + 1] = static_cast<sf::Uint8>((i / float(WIDTH)) * 255); // G Channel
			pixels[(i + j * WIDTH) * 4 + 2] = 0; // B Channel
			pixels[(i + j * WIDTH) * 4 + 3] = 255; // A Channel
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	window.setTitle(sf::String{ std::to_string(elapsedTime) + " ms" });


	targetTexture.update(pixels);

	sf::Sprite sprite;
	sprite.setTexture(targetTexture);

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

		ImGui::Begin("Sample window"); // begin window

									   // Background color edit
		if (ImGui::ColorEdit3("Background color", color)) {
			// this code gets called if color value changes, so
			// the background color is upgraded automatically!
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}

		// Window title text edit
		ImGui::InputText("Window title", windowTitle, 255);

		if (ImGui::Button("Update window title")) {
			// this code gets if user clicks on the button
			// yes, you could have written if(ImGui::InputText(...))
			// but I do this to show how buttons work :)
			window.setTitle(windowTitle);
		}
		ImGui::End(); // end window

		window.clear();
		window.draw(sprite);
		ImGui::SFML::Render(window);

		window.display();
	}

	return 0;
}