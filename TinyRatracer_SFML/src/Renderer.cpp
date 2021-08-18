#include "Renderer.h"
#include "Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, 
	previewHeight{previewHeight}, previewWidth { previewWidth },
	mCameraPosition{0.0f,0.0f,0.0f}
{
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere> & scene, 
	const std::vector<Light>& lights, bool isPreview) const
{
	// ����� Scene�� Sphere �ϳ��� �ִٰ� ����
	unsigned int renderW = 0;
	unsigned int renderH = 0;
	if (isPreview)
	{
		renderW = previewWidth;
		renderH = previewHeight;
	}
	else
	{
		renderW = width;
		renderH = height;
	}

	sf::Clock clock;
	for (size_t j = 0; j < renderH; j++) {
		for (size_t i = 0; i < renderW; i++) {
			frameBuffer[i + j * renderW] = Vec3f(j / float(renderH), i / float(renderW), 0);
			float x = (2 * (i + 0.5f) / (float)renderW - 1) * tan(fov / 2.0f)*renderW / (float)renderH;
			float y = -(2 * (j + 0.5f) / (float)renderH - 1) * tan(fov / 2.0f);
			Vec3f dir = Vec3f(x, y, -1).normalize();
			frameBuffer[i + j * renderW] = CastRay(mCameraPosition, dir, scene, lights); //ī�޶�� mCameraPosition�� ��ġ
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

bool Renderer::EditCameraPosition()
{
	return ImGui::DragFloat3("Camera position", &this->mCameraPosition.x);
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, const std::vector<Sphere> & scene, 
	const std::vector<Light>& lights) const
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{ };
	bool filled = false;
	for (const Sphere& s : scene)
	{
		if (s.RayIntersect(origin, direction, sphereDist))
		{
			// ���� �ȼ� ������ �ܼ� sphere�� ������ �ƴ� dot(n,l)�� ���� diffuse ����
			Vec3f point = origin + direction * sphereDist;
			// point-s.GetCenter() = normal direction of sphere
			float diffuseIntensity = CalculateLighting(lights, point, (point - s.GetCenter()).normalize());
			fillColor = s.GetColor() * diffuseIntensity;
			Utility::SaturateColor(fillColor);
			filled = true;
		}
	}

	if (!filled) // ��� ������ �ε����� ����
	{
		return Vec3f(0.2f, 0.7f, 0.8f);
	}
	else // ����� ���� �ε��� (fillcolor�� ���� ����� ���� ����)
	{
		return fillColor;
	}

}

float Renderer::CalculateLighting(const std::vector<Light>& lights, const Vec3f & point, const Vec3f & normal) const
{
	float diffuseIntensity = 0;
	for (const Light& light : lights) //scene�� ��� light�� ����
	{
		Vec3f lightDir = (light.GetPosition() - point).normalize(); //point�� ���� ��������,
		diffuseIntensity += light.GetIntensity() * std::max(0.f, lightDir*normal); // dot(n,l)�� ����� ����
	}
	return diffuseIntensity; //����� light color�� �����Ƿ� intensity�� return
}