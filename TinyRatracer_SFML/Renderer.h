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
	Vec3f mCameraPosition;

	Scene mScene;

public:
	Renderer(unsigned int w, unsigned int h, float fov);
	void SetScene(const Scene& scene);
	void Render(std::vector<Vec3f>& frameBuffer);

	void UpdateCamPosition(float dt, const Vec3f& dir);
private:
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction);
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		Vec3f& hit, Vec3f& normal, Material& material);

	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;

};