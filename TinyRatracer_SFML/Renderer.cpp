#include "Renderer.h"
#include "Utility.h"

#include <algorithm>
#include <vector>

Renderer::Renderer(unsigned int w, unsigned int h, float fov, size_t maxDepth)
	:width{ w }, height{ h }, fov{ fov },
	mCameraPosition{0.0f,0.0f,0.0f},
	maximumDepth{maxDepth}
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
			frameBuffer[i + j * width] = CastRay(mCameraPosition, dir, 0); //카메라는 0,0,0에 위치
		}
	}
}

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, size_t currentDepth)
{
	Vec3f hit, normal;
	Material material;
	
	if(currentDepth > maximumDepth || !SceneIntersect(origin, direction, hit, normal, material))
	{ 
		return Vec3f{ 0.2f, 0.7f, 0.8f };
	}
	// 교차하는 물체가 있는경우 hit,normal,material 정보가 저장됨

	//---Reflection
	Vec3f reflectDir = Reflect(direction, normal).normalize();
	Vec3f reflectOrigin = reflectDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	// 부딪힌 점의 색상은 그 점에서 reflection 방향으로 다시 빛을 쏘아 intersection한 곳의 색상.
	// 그 intersection의 색상은 다시 reflection 방향으로 빛을 recursive하게 쏘아 계산.
	// depth는 이렇게 몇번까지 intersection 계산을 한 것인지 제어함
	Vec3f reflectColor = CastRay(reflectOrigin, reflectDir, currentDepth + 1);
	
	//---Refraction
	Vec3f refractDir = Refract(direction, normal, material.GetRefractiveIndex()).normalize();
	Vec3f refractOrigin = refractDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	Vec3f refractColor = CastRay(refractOrigin, refractDir, currentDepth + 1);

	float diffuseIntensity = 0;
	float specularIntensity = 0;
	
	// Right side evaluated once
	for (const Light& light : mScene.GetLights())
	{
		Vec3f lightDir = (light.GetPosition() - hit).normalize();
		float lightDist = (light.GetPosition() - hit).norm();

		//Shadow evaluation
		Vec3f shadowOrigin = (lightDir * normal < 0) ? hit - normal * 1e-3 : hit + normal * 1e-3;
		Vec3f shadowLightDir = (light.GetPosition() - shadowOrigin).normalize();

		// 지금 그리려는 픽셀에서 빛 방향으로 다시 ray 발사
		Vec3f shadowHit, shadowNormal;
		Material tempMat;
		if (SceneIntersect(shadowOrigin, shadowLightDir, shadowHit, shadowNormal, tempMat)
			&& lightDist > (shadowHit - shadowOrigin).norm()) //다른 물체와 먼저 교차한 경우
		{
			continue;
		}

		diffuseIntensity += light.GetIntensity() * std::max(0.0f, lightDir*normal);
		specularIntensity += std::powf(std::max(0.0f, Reflect(lightDir, normal)*direction), material.GetSpecularExponent())
							*light.GetIntensity();
	}

	Vec4f materialAlbedo = material.GetAlbedo();
	Vec3f color = material.GetDiffuseColor() * diffuseIntensity * materialAlbedo[0] // diffuse term
		+ Vec3f{ 1.0f, 1.0f, 1.0f }*specularIntensity * materialAlbedo[1] // specular term
		+ reflectColor * materialAlbedo[2] // reflect term
		+ refractColor * materialAlbedo[3]; // refract term

	Utility::SaturateColor(color);
	return color;
}

bool Renderer::SceneIntersect(const Vec3f & origin, const Vec3f direction,
	Vec3f & hit, Vec3f & normal, Material & material)
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;

	for (const Sphere& s : mScene.GetObjects()) // Right side evaluated once
	{
		if (s.RayIntersect(origin, direction, sphereDist))
		{
			hit = origin + direction * sphereDist;
			normal = (hit - s.GetCenter()).normalize();
			material = s.GetMaterial();
		}
	}

	// Add checkerboard
	float checkerboard_dist = std::numeric_limits<float>::max();
	if (fabs(direction.y) > 1e-3) {
		float d = -(origin.y + 4) / direction.y; // the checkerboard plane has equation y = -4
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z<-10 && pt.z>-30 && d < sphereDist) {
			checkerboard_dist = d;
			hit = pt;
			normal = Vec3f(0, 1, 0);
			Vec3f diffuseColor = (int(.5*hit.x + 1000) + int(.5*hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, .7, .3);
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}
	return std::min(sphereDist, checkerboard_dist) < 1000;
}

Vec3f Renderer::Reflect(const Vec3f & l, const Vec3f & n) const
{
	return l - n * 2.0f*(n*l);
}

Vec3f Renderer::Refract(const Vec3f& I, const Vec3f& N, const float refractiveIndex) const
{
	float cosi = -std::max(-1.0f, std::min(1.0f, I*N));
	float etai = 1, etat = refractiveIndex;
	Vec3f n = N;
	if (cosi < 0)
	{
		cosi = -cosi;
		std::swap(etai, etat);
		n = -N;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta*(1 - cosi * cosi);
	//Step 8 메모 참조
	return k < 0 ? Vec3f{ 0, 0, 0 } : I * eta + n * (eta*cosi - sqrtf(k));
}


void Renderer::UpdateCamPosition(float dt, const Vec3f& dir)
{
	mCameraPosition = mCameraPosition + dir * dt * 5.0f;
}
