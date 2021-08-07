#include "Renderer.h"

Renderer::Renderer(unsigned int w, unsigned int h, float fov)
	:width{ w }, height{ h }, fov{ fov },
	mCameraPosition{0.0f,0.0f,0.0f}
{
}

void Renderer::Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere>& scene)
{
	// 현재는 Scene에 Sphere 하나만 있다고 가정

	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++) {
			frameBuffer[i + j * width] = Vec3f(j / float(height), i / float(width), 0);
			float x = (2 * (i + 0.5f) / (float)width - 1) * tan(fov / 2.0f)*width / (float)height;
			float y = -(2 * (j + 0.5f) / (float)height - 1) * tan(fov / 2.0f);
			Vec3f dir = Vec3f(x, y, -1).normalize();
			frameBuffer[i + j * width] = CastRay(mCameraPosition, dir, scene); //카메라는 0,0,0에 위치
		}
	}
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, const std::vector<Sphere>& scene)
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;
	for (const Sphere& s : scene)
	{
		if (s.rayIntersect(origin, direction, sphereDist))
		{
			fillColor = s.GetColor();
			filled = true;
		}
	}

	if (!filled)
		return Vec3f(0.2f, 0.7f, 0.8f); // Not intersect
	else
		return fillColor; // Intersect, closest
}

void Renderer::UpdateCamPosition(float dt, Vec3f dir)
{
	mCameraPosition = mCameraPosition + dir * dt;
}
