#include "Renderer.h"
#include "Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, float fov)
	:width{ w }, height{ h }, fov{ fov },
	mCameraPosition{0.0f,0.0f,0.0f}
{
}

void Renderer::Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere>& scene, const std::vector<Light>& lights)
{
	// 현재는 Scene에 Sphere 하나만 있다고 가정

	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++) {
			frameBuffer[i + j * width] = Vec3f(j / float(height), i / float(width), 0);
			float x = (2 * (i + 0.5f) / (float)width - 1) * tan(fov / 2.0f)*width / (float)height;
			float y = -(2 * (j + 0.5f) / (float)height - 1) * tan(fov / 2.0f);
			Vec3f dir = Vec3f(x, y, -1).normalize();
			frameBuffer[i + j * width] = CastRay(mCameraPosition, dir, scene, lights); //카메라는 0,0,0에 위치
		}
	}
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, 
	const std::vector<Sphere>& scene, const std::vector<Light>& lights) const
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;
	for (const Sphere& s : scene)
	{
		if (s.RayIntersect(origin, direction, sphereDist))
		{
			Vec3f point = origin + direction * sphereDist;
			// point-s.GetCenter() = normal direction of sphere
			float diffuseIntensity = CalculateLighting(lights, point, (point - s.GetCenter()).normalize()); 
			fillColor = s.GetColor() * diffuseIntensity;
			Utility::SaturateColor(fillColor);
			filled = true;
		}
	}

	if (!filled)
		return Vec3f(0.2f, 0.7f, 0.8f); // Not intersect
	else
		return fillColor; // Intersect, closest
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


void Renderer::UpdateCamPosition(float dt, const Vec3f& dir)
{
	mCameraPosition = mCameraPosition + dir * dt;
}
