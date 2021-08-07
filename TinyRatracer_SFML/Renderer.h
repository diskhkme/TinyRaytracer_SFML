#pragma once

#include "Sphere.h"

class Renderer
{
private:
	const unsigned int width;
	const unsigned int height;
	const float fov;
	Vec3f mCameraPosition;

public:
	Renderer(unsigned int w, unsigned int h, float fov);
	void Render(std::vector<Vec3f>& frameBuffer, const Sphere& sphere);
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction, const Sphere& sphere);

	void UpdateCamPosition(float dt, Vec3f dir);
private:

};