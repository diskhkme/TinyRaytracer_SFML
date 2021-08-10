#pragma once

#include "Sphere.h"
#include "Light.h"
#include "Scene.h"

class Renderer
{
private:
	const unsigned int width;
	const unsigned int height;
	const float fov;
	const size_t maximumDepth;
	Vec3f mCameraPosition;

	Scene mScene;

public:
	Renderer(unsigned int w, unsigned int h, float fov, size_t maxDepth);
	void SetScene(const Scene& scene);
	void Render(std::vector<Vec3f>& frameBuffer);

	void UpdateCamPosition(float dt, const Vec3f& dir);
private:
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction, size_t currentDepth);
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		Vec3f& hit, Vec3f& normal, Material& material);

	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;
	Vec3f Refract(const Vec3f& I, const Vec3f& N, const float refractiveIndex) const;

};