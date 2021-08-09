#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"

class Window : public sf::NonCopyable
{
public:
	Window(unsigned int width, unsigned int height, float fov);
	~Window();
	void AddSphere(const Sphere& s);
	void AddLight(const Light& l);
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time elapsedTime);
	void Render();

	void UpdateStatistics(sf::Time elapsedTime);
	void HandlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	static const sf::Time timePerFrame;

	sf::RenderWindow mWindow;

	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	size_t mStatisticsNumFrames;

	Renderer mRenderer;
	// Renderer outputs
	sf::Texture mDisplayTexture;
	sf::Sprite mDisplaySprite;
	std::vector<Vec3f> mFramebuffer;
	sf::Uint8* mPixels;

	// Scene 
	std::vector<Sphere> mScene;
	std::vector<Light> mLight;

	// Simple Camera Moving from Input WASD
	bool mIsMovingForward;
	bool mIsMovingBackward;
	bool mIsMovingLeft;
	bool mIsMovingRight;
};
