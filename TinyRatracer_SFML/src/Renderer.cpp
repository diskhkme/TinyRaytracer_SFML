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
	// 현재는 Scene에 Sphere 하나만 있다고 가정
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
			frameBuffer[i + j * renderW] = CastRay(mCameraPosition, dir, scene, lights); //카메라는 mCameraPosition에 위치
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
			// 이제 픽셀 색상은 단순 sphere의 색상이 아닌 dot(n,l)로 계산된 diffuse 색상
			Vec3f point = origin + direction * sphereDist;
			// point-s.GetCenter() = normal direction of sphere
			float diffuseIntensity = CalculateLighting(lights, point, (point - s.GetCenter()).normalize());
			fillColor = s.GetColor() * diffuseIntensity;
			Utility::SaturateColor(fillColor);
			filled = true;
		}
	}

	if (!filled) // 어떠한 구에도 부딪히지 않음
	{
		return Vec3f(0.2f, 0.7f, 0.8f);
	}
	else // 어딘가의 구에 부딪힘 (fillcolor는 가장 가까운 구의 색상)
	{
		return fillColor;
	}

}

float Renderer::CalculateLighting(const std::vector<Light>& lights, const Vec3f & point, const Vec3f & normal) const
{
	float diffuseIntensity = 0;
	for (const Light& light : lights) //scene의 모든 light에 대해
	{
		Vec3f lightDir = (light.GetPosition() - point).normalize(); //point와 빛의 방향으로,
		diffuseIntensity += light.GetIntensity() * std::max(0.f, lightDir*normal); // dot(n,l)의 결과를 누적
	}
	return diffuseIntensity; //현재는 light color가 없으므로 intensity만 return
}