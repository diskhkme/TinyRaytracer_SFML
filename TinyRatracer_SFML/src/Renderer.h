#pragma once

#include "Sphere.h"
#include "Light.h"

#include "SFML/Graphics.hpp"

class Renderer
{
private:
	const unsigned int width;
	const unsigned int height;
	const unsigned int previewWidth;
	const unsigned int previewHeight;
	float fov;

	// Camera
	Vec3f mOrbitCameraParameter; // (x,y,z) => (r, yaw, pitch)
	Vec3f mCameraPosition;
	Vec3f mCameraForward;
	Vec3f mCameraRight;
	Vec3f mCameraUp;

public:
	Renderer(unsigned int w, unsigned int h, float fov,
			unsigned int previewWidth, unsigned int previewHeight);
	sf::Int32 Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere>& scene,
		const std::vector<Light>& lights, bool isPreview) const;

	bool EditCamera();
	
private:
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction, const std::vector<Sphere>& scene,
		const std::vector<Light>& lights) const;
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		const std::vector<Sphere>& scene, Vec3f& hit, Vec3f& normal, Material& material) const;

	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;

	void UpdateCamera();
};