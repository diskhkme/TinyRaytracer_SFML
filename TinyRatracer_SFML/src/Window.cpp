#include "Window.h"
#include "Utility.h"

const sf::Time Window::timePerFrame = sf::seconds(1.0f / 60.0f);

Window::Window(unsigned int width, unsigned int height, float fov, size_t maxDepth)
	: mWindow{ sf::VideoMode(width, height), "My Application", sf::Style::Close },
	mRenderer{width, height, fov, maxDepth },
	mFramebuffer{width*height},
	mDisplayTexture{}, mDisplaySprite{},
	mFont{}, mStatisticsText{}
{
	mFont.loadFromFile("resources/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	if (!mDisplayTexture.create(width, height))
	{
		std::cout << "Display Texture Initialization Failed!!" << std::endl;
		assert(1);
	}

	mDisplayTexture.setSmooth(true);

	mPixels = new sf::Uint8[width * height * 4];

	mDisplaySprite.setTexture(mDisplayTexture);
}

Window::~Window()
{
	delete[] mPixels;
}

void Window::SetScene(const Scene* const scene)
{
	mRenderer.SetScene(scene);
}

void Window::Run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		ProcessEvents();

		if (startRender)
		{
			sf::Time renderTime = clock.restart();
			Render();
			renderTime = clock.getElapsedTime();
			UpdateStatistics(renderTime);
					   
			mWindow.draw(mDisplaySprite);
			mWindow.draw(mStatisticsText);
			mWindow.display();

			std::cout << "Render Complete!" << std::endl;
			startRender = false;
		}
	}
}

void Window::ProcessEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			HandlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			HandlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Window::Update(sf::Time elapsedTime)
{
	// Scene update?
}

void Window::Render()
{
	mWindow.clear();

	mRenderer.Render(mFramebuffer);
	Utility::ConvertPixelsFromVector(mFramebuffer, mPixels);
	mDisplayTexture.update(mPixels);
}

void Window::UpdateStatistics(sf::Time renderTime)
{
	mStatisticsText.setString(
		"Rendering takes = " + std::to_string(renderTime.asMilliseconds()) + " ms");
}

void Window::HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Space && isPressed)
		startRender = true;
}

