#include "Renderer.h"
#include "Utility.h"

#include <algorithm>
#include <vector>

Renderer::Renderer(unsigned int w, unsigned int h, float fov)
	:width{ w }, height{ h }, fov{ fov },
	mCameraPosition{0.0f,0.0f,0.0f}
{
}

void Renderer::SetScene(const Scene & scene)
{
	mScene = scene;
}

void Renderer::Render(std::vector<Vec3f>& frameBuffer)
{
	// 현재는 Scene에 Sphere 하나만 있다고 가정

	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++) {
			frameBuffer[i + j * width] = Vec3f(j / float(height), i / float(width), 0);
			float x = (2 * (i + 0.5f) / (float)width - 1) * tan(fov / 2.0f)*width / (float)height;
			float y = -(2 * (j + 0.5f) / (float)height - 1) * tan(fov / 2.0f);
			Vec3f dir = Vec3f(x, y, -1).normalize();
			frameBuffer[i + j * width] = CastRay(mCameraPosition, dir); //카메라는 0,0,0에 위치
		}
	}
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction)
{
	Vec3f hit, normal;
	Material material;
	
	if(!SceneIntersect(origin, direction, hit, normal, material))
	{ 
		return Vec3f{ 0.2f, 0.7f, 0.8f };
	}
	// 교차하는 물체가 있는경우 hit,normal,material 정보가 저장됨

	float diffuseIntensity = 0;
	float specularIntensity = 0;
	
	// Right side evaluated once
	for (const Light& light : mScene.GetLights())
	{
		Vec3f lightDir = (light.GetPosition() - hit).normalize();

		diffuseIntensity += light.GetIntensity() * std::max(0.0f, lightDir*normal);
		specularIntensity += std::powf(std::max(0.0f, Reflect(lightDir, normal)*direction), material.GetSpecularExponent())
							*light.GetIntensity();
	}

	Vec2f materialAlbedo = material.GetAlbedo();
	Vec3f color = material.GetDiffuseColor() * diffuseIntensity * materialAlbedo[0] // diffuse term
		+ Vec3f{ 1.0f, 1.0f, 1.0f }*specularIntensity * materialAlbedo[1]; // specular term
	Utility::SaturateColor(color);
	return color;
}

bool Renderer::SceneIntersect(const Vec3f & origin, const Vec3f direction,
	Vec3f & hit, Vec3f & normal, Material & material)
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;

	// Right side evaluated once
	for (const Sphere& s : mScene.GetObjects())
	{
		if (s.RayIntersect(origin, direction, sphereDist))
		{
			hit = origin + direction * sphereDist;
			normal = (hit - s.GetCenter()).normalize();
			material = s.GetMaterial();
		}
	}

	return sphereDist < 1000; // 최대 거리. Camera의 far plane이면 적절하겠죠?
}

Vec3f Renderer::Reflect(const Vec3f & l, const Vec3f & n) const
{
	return l - n * 2.0f*(n*l);
}


void Renderer::UpdateCamPosition(float dt, const Vec3f& dir)
{
	mCameraPosition = mCameraPosition + dir * dt;
}
