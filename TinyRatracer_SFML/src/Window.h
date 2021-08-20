#pragma once

#include <SFML/Graphics.hpp>

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

#include "Renderer/Renderer.h"
#include "Scene/SceneManager.h"

class Window : public sf::NonCopyable
{
public:
	Window(unsigned int width, unsigned int height, size_t maxDepth,
			unsigned int previewWidth, unsigned int previewHeight);
	~Window();
	void Run();
	void SetScene(SceneManager* const scene);

private:
	void SetRenderGUI();
	void SetEditorGUI();
	void RenderImage();

private:
	sf::RenderWindow mWindow;

	// Render Managing
	Renderer mRenderer;
	sf::Int32 renderTime = 0;
	sf::Int32 updateTime = 0;
	bool bPreviewOn = true;

	// Render output
	sf::Sprite mDisplaySprite;
	sf::Texture mTexture;
	std::vector<Vec3f> mFramebuffer;
	sf::Uint8* mPixels;

	// Preview output
	sf::Texture mPreviewTexture; 
	std::vector<Vec3f> mPreviewFramebuffer; 
	sf::Uint8* mPreviewPixels;
};