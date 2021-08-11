#pragma once

#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "scene/Scene.h"

class Window : public sf::NonCopyable
{
public:
	Window(unsigned int width, unsigned int height, float fov, size_t maxDepth);
	~Window();

	void SetScene(const Scene* const scene);
	void Run();

private:
	void ProcessEvents();
	void Update(sf::Time elapsedTime);
	void Render();

	void UpdateStatistics(sf::Time renderTime);
	void HandlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	static const sf::Time timePerFrame;

	sf::RenderWindow mWindow;

	sf::Font mFont;
	sf::Text mStatisticsText;

	Renderer mRenderer;
	// Renderer outputs
	sf::Texture mDisplayTexture;
	sf::Sprite mDisplaySprite;
	std::vector<Vec3f> mFramebuffer;
	sf::Uint8* mPixels;

	// Single frame render signal
	bool startRender = true;
};