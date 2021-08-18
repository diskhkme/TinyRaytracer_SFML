#include "Window.h"
#include "Utility.h"

Window::Window(unsigned int width, unsigned int height, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	: mWindow{ sf::VideoMode{width, height}, "TinyRenderer", sf::Style::Close },
	mRenderer{ width, height, fov, previewWidth, previewHeight },
	mDisplaySprite{},
	mFramebuffer{ width*height }, mTexture{},
	mPreviewFramebuffer{ previewWidth*previewHeight }, mPreviewTexture{}
{
	assert(mTexture.create(width, height));
	assert(mPreviewTexture.create(previewWidth, previewHeight));

	mPixels = new sf::Uint8[width * height * 4];
	mPreviewPixels = new sf::Uint8[previewWidth * previewHeight * 4];

	ImGui::SFML::Init(mWindow);
}

Window::~Window()
{
	delete[] mPixels;
	delete[] mPreviewPixels;
}

void Window::Run()
{
	sf::Clock clock;
	while (mWindow.isOpen())
	{
		sf::Event event;
		while (mWindow.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				mWindow.close();
		}
		ImGui::SFML::Update(mWindow, clock.restart());

		// Render preview
		if (bPreviewOn)
		{
			RenderImage();
		}
			
		// Window GUI
		SetRenderGUI();

		// Edit GUI
		SetEditorGUI();

		mWindow.clear();
		mWindow.draw(mDisplaySprite);
		ImGui::SFML::Render(mWindow);

		mWindow.display();
	}
}

void Window::SetRenderGUI()
{
	//---Render Menu
	ImGui::Begin("Render Menu");
	ImGui::Text("Press Render button to render");
	if (ImGui::Button("Render")) {
		bPreviewOn = false;
		RenderImage();
	}
	ImGui::Text("Rendering: %d ms", renderTime);
	ImGui::Text("Update Display: %d ms", updateTime);
	ImGui::End();
}

void Window::SetEditorGUI()
{
	//---geometry menu
	ImGui::Begin("Edit Sphere");
	ImGui::Text("if values changed, preview screen will be shown");
	ImGui::Text("after changing the values, press render button again");
	bool e1 = ImGui::ColorEdit3("sphere color", mSphere.GetColor());
	bool e2 = ImGui::DragFloat3("sphere position", mSphere.GetCenter());
	bool e3 = ImGui::SliderFloat("sphere radius", mSphere.GetRadius(),0.1f, 100.0f);
	bPreviewOn = e1 | e2 | e3;
	ImGui::End();
}

void Window::RenderImage()
{
	if (bPreviewOn)
	{
		// No time calculation for preview
		mRenderer.Render(mPreviewFramebuffer, mSphere, bPreviewOn);
		Utility::ConvertPixelsFromVector(mPreviewFramebuffer, mPreviewPixels);
		mPreviewTexture.update(mPreviewPixels);

		// Stretch preview texture
		mDisplaySprite.setTexture(mPreviewTexture, true);
		mDisplaySprite.setScale((float)mTexture.getSize().x / mPreviewTexture.getSize().x, 
								(float)mTexture.getSize().y / mPreviewTexture.getSize().y);
	}
	else
	{
		renderTime = mRenderer.Render(mFramebuffer, mSphere, bPreviewOn);
		sf::Clock clock;
		Utility::ConvertPixelsFromVector(mFramebuffer, mPixels);
		mTexture.update(mPixels);
		updateTime = clock.getElapsedTime().asMilliseconds();

		mDisplaySprite.setTexture(mTexture, true);
		mDisplaySprite.setScale(1.0f, 1.0f);
	}
}


