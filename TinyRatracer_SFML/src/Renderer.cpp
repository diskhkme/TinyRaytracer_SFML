#include "Renderer.h"

Renderer::Renderer(unsigned int w, unsigned int h, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, 
	previewHeight{previewHeight}, previewWidth { previewWidth }
{
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere> & scene, bool isPreview)
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
			frameBuffer[i + j * renderW] = CastRay(Vec3f(0, 0, 0), dir, scene); //ī�޶�� 0,0,0�� ��ġ
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, const std::vector<Sphere> & scene)
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{ };
	bool filled = false;
	for (const Sphere& s : scene)
	{
		if (s.rayIntersect(origin, direction, sphereDist))
		{
			fillColor = s.GetColor();
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