#pragma once

#include "Sphere.h"
#include "Light.h"

class Renderer
{
private:
	const unsigned int width;
	const unsigned int height;
	const float fov;
	Vec3f mCameraPosition;

public:
	Renderer(unsigned int w, unsigned int h, float fov);
	void Render(std::vector<Vec3f>& frameBuffer, 
		const std::vector<Sphere>& scene, const std::vector<Light>& lights);

	void UpdateCamPosition(float dt, const Vec3f& dir);
private:
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction,
		const std::vector<Sphere>& scene, const std::vector<Light>& lights);
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		const std::vector<Sphere>& scene, Vec3f& hit, Vec3f& normal, Material& material);

	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;

};