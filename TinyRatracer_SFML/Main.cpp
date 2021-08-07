#include <SFML/Graphics.hpp>

int main()
{
	constexpr int WIDTH = 1024;
	constexpr int HEIGHT = 768;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
	window.setVerticalSyncEnabled(true);

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
			pixels[(i + j * WIDTH) * 4 + 0] = static_cast<sf::Uint8>((j / float(HEIGHT))*255); // R Channel
			pixels[(i + j * WIDTH) * 4 + 1] = static_cast<sf::Uint8>((i / float(WIDTH))*255); // G Channel
			pixels[(i + j * WIDTH) * 4 + 2] = 0; // B Channel
			pixels[(i + j * WIDTH) * 4 + 3] = 255; // A Channel
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	window.setTitle(sf::String{ std::to_string(elapsedTime) + " ms" });


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

	return 0;
}