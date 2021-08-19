#pragma once

#include "Sphere.h"
#include "Light.h"
#include "SceneManager.h"

#include "SFML/Graphics.hpp"

class Renderer
{
private:
	// Window
	const unsigned int width;
	const unsigned int height;
	const unsigned int previewWidth;
	const unsigned int previewHeight;
	float fov;

	// Scene
	SceneManager mScene;

	// Camera
	Vec3f mOrbitCameraParameter; // (x,y,z) => (r, yaw, pitch)
	Vec3f mCameraPosition;
	Vec3f mCameraForward;
	Vec3f mCameraRight;
	Vec3f mCameraUp;

public:
	Renderer(unsigned int w, unsigned int h, float fov,
			unsigned int previewWidth, unsigned int previewHeight);
	sf::Int32 Render(std::vector<Vec3f>& frameBuffer, bool isPreview) const;

	bool EditorGUI();
	void SetScene(const SceneManager& scene);
	
private:
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction) const;
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		Vec3f& hit, Vec3f& normal, Material& material) const;
	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;

	bool EditCamera();
	void UpdateCamera();
};