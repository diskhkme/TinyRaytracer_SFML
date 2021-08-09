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
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction, 
		const std::vector<Sphere>& scene, const std::vector<Light>& lights) const;
	float CalculateLighting(const std::vector<Light>& lights, 
		const Vec3f & point, const Vec3f & normal) const;

	void UpdateCamPosition(float dt, const Vec3f& dir);
private:

};