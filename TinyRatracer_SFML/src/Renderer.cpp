#include "Renderer.h"

Renderer::Renderer(unsigned int w, unsigned int h, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, 
	previewHeight{previewHeight}, previewWidth { previewWidth }
{
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, const Sphere & sphere, bool isPreview)
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
			frameBuffer[i + j * renderW] = CastRay(Vec3f(0, 0, 0), dir, sphere); //카메라는 0,0,0에 위치
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, const Sphere& sphere)
{
	float sphereDist = std::numeric_limits<float>::max();
	if (!sphere.rayIntersect(origin, direction, sphereDist))
	{
		return Vec3f{0.2f, 0.7f, 0.8f};
	}
	return sphere.GetColor(); // Intersect
}