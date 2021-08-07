#include "Window.h"
#include "Utility.h"

const sf::Time Window::timePerFrame = sf::seconds(1.0f / 60.0f);

Window::Window(unsigned int width, unsigned int height, float fov)
	: mWindow{ sf::VideoMode(width, height), "My Application", sf::Style::Close },
	mRenderer{width, height, fov},
	mFramebuffer{width*height},
	mDisplayTexture{}, mDisplaySprite{},
	mFont{}, mStatisticsText{}, mStatisticsUpdateTime{},
	mStatisticsNumFrames{ 0 }
{
	mFont.loadFromFile("Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	if (mDisplayTexture.create(width, height))
	{
		std::cout << "Display Texture Initialization Failed!!" << std::endl;
		assert(1);
	}

	mPixels = new sf::Uint8[width * height * 4];
}

Window::~Window()
{
	delete mPixels;
}

void Window::Run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			ProcessEvents();
			Update(timePerFrame);
		}

		UpdateStatistics(elapsedTime);
		Render();
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
	Vec3f camMovingDir{ 0.0f,0.0f,0.0f };
	if (mIsMovingForward)
		camMovingDir = camMovingDir + Vec3f{ 0.0f,0.0f,-1.0f };
	if (mIsMovingBackward)
		camMovingDir = camMovingDir + Vec3f{ 0.0f,0.0f, 1.0f };
	if (mIsMovingLeft)
		camMovingDir = camMovingDir + Vec3f{ -1.0f,0.0f, 0.0f };
	if (mIsMovingRight)
		camMovingDir = camMovingDir + Vec3f{  1.0f,0.0f, 0.0f };

	if (mIsMovingForward || mIsMovingBackward || mIsMovingLeft || mIsMovingRight)
	{
		camMovingDir.normalize();
		mRenderer.UpdateCamPosition(elapsedTime.asSeconds(), camMovingDir);
	}
	
}

void Window::Render()
{
	mWindow.clear();

	mRenderer.Render(mFramebuffer, mSphere);
	Utility::ConvertPixelsFromVector(mFramebuffer, mPixels);
	mDisplayTexture.update(mPixels);
	mDisplaySprite.setTexture(mDisplayTexture);

	mWindow.draw(mDisplaySprite);
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Window::UpdateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		sf::Int64 averageTimePerUpdate = mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames;
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(averageTimePerUpdate) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Window::HandlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		mIsMovingForward = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingBackward = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
}

