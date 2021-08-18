#include "Window.h"
#include "Utility.h"

Window::Window(unsigned int width, unsigned int height, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	: mWindow{ sf::VideoMode{width, height}, "TinyRenderer", sf::Style::Close },
	mRenderer{ width, height, fov, previewWidth, previewHeight },
	mDisplaySprite{},
	mFramebuffer{ width*height }, mTexture{},
	mPreviewFramebuffer{ previewWidth*previewHeight }, mPreviewTexture{},
	mScene{}, mLights{}
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

void Window::AddSphere(const Sphere & s)
{
	mScene.emplace_back(s);
}

void Window::AddLight(const Light & l)
{
	mLights.emplace_back(l);
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
	//---edit scene & light
	ImGui::Begin("Edit Scene");
	ImGui::Text("if values changed, preview screen will be shown");
	ImGui::Text("after changing the values, press render button again");
	for (size_t i = 0; i < mScene.size(); i++)
	{
		std::string label = "Sphere" + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			bPreviewOn |= mScene[i].EditSphere();
			ImGui::TreePop();
		}
	}
	ImGui::Separator();
	for (size_t i = 0; i < mLights.size(); i++)
	{
		std::string label = "Light" + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			bPreviewOn |= mLights[i].EditLight();
			ImGui::TreePop();
		}
	}
	ImGui::End();

	//---edit Camera
	ImGui::Begin("Edit Camera");
	ImGui::Text("if values changed, preview screen will be shown");
	ImGui::Text("after changing the values, press render button again");
	if (mRenderer.EditCamera())
	{
		bPreviewOn = true;
	}
	ImGui::End();

	
}

void Window::RenderImage()
{
	if (bPreviewOn)
	{
		// No time calculation for preview
		mRenderer.Render(mPreviewFramebuffer, mScene, mLights, bPreviewOn);
		Utility::ConvertPixelsFromVector(mPreviewFramebuffer, mPreviewPixels);
		mPreviewTexture.update(mPreviewPixels);

		// Stretch preview texture
		mDisplaySprite.setTexture(mPreviewTexture, true);
		mDisplaySprite.setScale((float)mTexture.getSize().x / mPreviewTexture.getSize().x, 
								(float)mTexture.getSize().y / mPreviewTexture.getSize().y);
	}
	else
	{
		renderTime = mRenderer.Render(mFramebuffer, mScene, mLights, bPreviewOn);
		sf::Clock clock;
		Utility::ConvertPixelsFromVector(mFramebuffer, mPixels);
		mTexture.update(mPixels);
		updateTime = clock.getElapsedTime().asMilliseconds();

		mDisplaySprite.setTexture(mTexture, true);
		mDisplaySprite.setScale(1.0f, 1.0f);
	}
}


