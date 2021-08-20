#include "Renderer.h"
#include "../Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, size_t maxDepth, size_t samplesPerPixel,
	unsigned int previewWidth, unsigned int previewHeight)
	: previewRenderParam{previewWidth, previewHeight, 1, 1},
	targetRenderParam{w, h, maxDepth, samplesPerPixel}
{
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, bool isPreview) const
{
	if (isPreview)
	{
		currentParam = previewRenderParam;
	}
	else
	{
		currentParam = targetRenderParam;
	}
	dist_t aspectRatio = static_cast<dist_t>(currentParam.widthResolution) / currentParam.heightResolution;

	sf::Clock clock;
	for (size_t j = 0; j < currentParam.heightResolution; j++) {
		for (size_t i = 0; i < currentParam.widthResolution; i++) {
			Vec3f color;
			for (size_t sample = 0; sample < currentParam.samplesPerPixel; sample++)
			{
				dist_t w = static_cast<dist_t>(i) + Utility::RandomValueZeroOne();
				dist_t h = static_cast<dist_t>(j) + Utility::RandomValueZeroOne();
				dist_t u = (2.0f * (w + 0.5f) / currentParam.widthResolution) - 1.0f;
				dist_t v = (2.0f * (h + 0.5f) / currentParam.heightResolution) - 1.0f;

				Ray ray = mScene->GetCamera().GetRay(u, v, aspectRatio);
				color = color + CastRay(ray, isPreview, 0);
			}

			frameBuffer[i + j * currentParam.widthResolution] = color * (1.0f / currentParam.samplesPerPixel);
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

bool Renderer::EditRenderer()
{
	//---edit scene & light
	ImGui::Begin("Edit Scene");
	ImGui::Text("if values changed, preview screen will be shown");
	ImGui::Text("after changing the values, press render button again");
	bool e1 = mScene->EditScene();
	ImGui::End();

	//---edit Camera
	ImGui::Begin("Edit Camera");
	ImGui::Text("if values changed, preview screen will be shown");
	ImGui::Text("after changing the values, press render button again");
	bool e2 = mScene->GetCamera().EditCamera();
	ImGui::End();

	ImGui::Begin("Render Options");
	ImGui::DragInt("Maximum depth", (int*)&targetRenderParam.maxDepth, 1.0f, 1, 100);
	ImGui::DragInt("Samples per pixel", (int*)&targetRenderParam.samplesPerPixel, 1.0f, 1, 100);
	ImGui::End();

	return e1 | e2;
}

void Renderer::SetScene(SceneManager* const scene)
{
	mScene = scene;
}

Vec3f Renderer::CastRay(const Ray& ray, bool isPreview, size_t currentDepth) const
{
	Hit hit;
	if (currentDepth > currentParam.maxDepth || !SceneIntersect(ray, hit))
	{
		return mScene->GetEnvironmentColor(ray.direction);
	}

	//---Reflection
	Vec3f reflectDir = Reflect(ray.direction, hit.normal).normalize();
	Vec3f reflectOrigin = reflectDir * hit.normal < 0 ? hit.point - hit.normal * EPS : hit.point + hit.normal * EPS;
	Ray reflectRay{ reflectOrigin,reflectDir };
	Vec3f reflectColor = CastRay(reflectRay, isPreview, currentDepth + 1);

	//---Refraction
	Vec3f refractDir = Refract(ray.direction, hit.normal, hit.material.GetRefractiveIndex()).normalize();
	Vec3f refractOrigin = refractDir * hit.normal < 0 ? hit.point - hit.normal * EPS : hit.point + hit.normal * EPS;
	Ray refractRay{ refractOrigin,refractDir };
	Vec3f refractColor = CastRay(refractRay, isPreview, currentDepth + 1);

	float diffuseIntensity = 0;
	float specularIntensity = 0;

	for (const Light* const light : mScene->GetLights())
	{
		Vec3f lightDir = (light->GetPosition() - hit.point).normalize();
		dist_t lightDist = (light->GetPosition() - hit.point).norm();

		//Shadow evaluation
		Vec3f shadowOrigin = (lightDir * hit.normal < 0) ? hit.point - hit.normal * EPS : hit.point + hit.normal * EPS;
		Vec3f shadowLightDir = (light->GetPosition() - shadowOrigin).normalize();
		Ray shadowRay{ shadowOrigin,shadowLightDir };

		// 지금 그리려는 픽셀에서 빛 방향으로 다시 ray 발사
		Hit shadowHit;
		if (SceneIntersect(shadowRay, shadowHit)
			&& lightDist > (shadowHit.point - shadowOrigin).norm()) //다른 물체와 먼저 교차한 경우
		{
			continue;
		}

		diffuseIntensity += light->GetIntensity() * std::max(0.0f, lightDir*hit.normal);
		specularIntensity += std::powf(std::max(0.0f, Reflect(lightDir, hit.normal)*ray.direction), hit.material.GetSpecularExponent())
							* light->GetIntensity();
	}

	Vec4f materialAlbedo = hit.material.GetAlbedo();
	Vec3f color = hit.material.GetDiffuseColor() * diffuseIntensity * materialAlbedo[0] // diffuse term
		+ Vec3f{ 1.0f, 1.0f, 1.0f }*specularIntensity * materialAlbedo[1] // specular term
		+ reflectColor * materialAlbedo[2] // reflect term
		+ refractColor * materialAlbedo[3]; // refract term
	Utility::SaturateColor(color);
	return color;
}

bool Renderer::SceneIntersect(const Ray& ray, Hit& hit) const
{
	for (const ModelBase* const m : mScene->GetObjects())
	{
		m->RayIntersect(ray, hit);
	}

	// Add checkerboard
	if (fabs(ray.direction.y) > 1e-3) {
		dist_t d = -(ray.origin.y + 4) / ray.direction.y; // the checkerboard plane has equation y = -4
		Vec3f pt = ray.origin + ray.direction * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z<6 && pt.z>-14 && d < hit.t) {
			hit.t = d;
			hit.point = pt;
			hit.normal = Vec3f(0, 1, 0);
			Vec3f diffuseColor = (int(.5f*hit.point.x + 1000) + int(.5f*hit.point.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, .7f, .3f);
			diffuseColor = diffuseColor * 0.3f;
			hit.material.SetDiffuseColor(diffuseColor);
		}
	}
	//return std::min(modelDist, checkerboard_dist) < 1000;
	return hit.t < 1000;
}

Vec3f Renderer::Reflect(const Vec3f & l, const Vec3f & n) const
{
	return l - n * 2.0f*(n*l);
}

Vec3f Renderer::Refract(const Vec3f& I, const Vec3f& N, const float etat, float etai) const
{
	float cosi = -std::max(-1.0f, std::min(1.0f, I*N));
	
	if (cosi < 0.0f)
	{
		return Refract(I, -N, etai, etat);
	}
	float eta = etai / etat;
	float k = 1.0f - eta * eta*(1.0f - cosi * cosi);

	return k < 0 ? Vec3f{ 1, 0, 0 } : I * eta + N * (eta*cosi - sqrtf(k));
}