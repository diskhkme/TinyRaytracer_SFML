#pragma once

#include <SFML/Graphics.hpp>

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

#include "Renderer.h"

class Window : public sf::NonCopyable
{
public:
	Window(unsigned int width, unsigned int height, float fov,
			unsigned int previewWidth, unsigned int previewHeight);
	~Window();
	void Run();

private:
	void SetRenderGUI();
	void SetEditorGUI();
	void RenderImage();

private:
	sf::RenderWindow mWindow;

	Renderer mRenderer;
	// Renderer outputs
	sf::Sprite mDisplaySprite;
	sf::Texture mTexture;
	std::vector<Vec3f> mFramebuffer;
	sf::Uint8* mPixels;
	// Preview
	sf::Texture mPreviewTexture; 
	std::vector<Vec3f> mPreviewFramebuffer; 
	sf::Uint8* mPreviewPixels;

	sf::Int32 renderTime = 0;
	sf::Int32 updateTime = 0;

	bool bPreviewOn = true;
	
	// Scene 
	Sphere mSphere = Sphere{ Vec3f{0.0f,0.0f,-5.0f},1.0f, Vec3f{0.4f, 0.4f, 0.3f} };

};