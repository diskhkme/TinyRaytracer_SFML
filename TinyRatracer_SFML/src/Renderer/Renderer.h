#pragma once

#include "../Scene/Sphere.h"
#include "../Scene/Light.h"
#include "../Scene/SceneManager.h"
#include "../Geometry.h"
#include "Ray.h"

#include "SFML/Graphics.hpp"

class Renderer
{
private:
	// Window
	const unsigned int width;
	const unsigned int height;
	const unsigned int previewWidth;
	const unsigned int previewHeight;
	size_t maxDepth;
	size_t currentMaxDepth;

	// Scene
	SceneManager* mScene;

public:
	Renderer(unsigned int w, unsigned int h, size_t maxDepth,
			unsigned int previewWidth, unsigned int previewHeight);
	sf::Int32 Render(std::vector<Vec3f>& frameBuffer, bool isPreview) const;

	bool EditorGUI();
	void SetScene(SceneManager* const scene);
	
private:
	Vec3f CastRay(const Ray& ray, bool isPreview, size_t currentDepth) const;
	bool SceneIntersect(const Ray& ray,	Hit& hit) const;
	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;
	Vec3f Refract(const Vec3f& I, const Vec3f& N, const float etat, float etai=1.0f) const;
};