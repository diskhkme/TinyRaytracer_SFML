#include "Renderer.h"
#include "../Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, float fov, size_t maxDepth,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, maxDepth{ maxDepth }, currentMaxDepth{maxDepth},
	previewHeight{previewHeight}, previewWidth { previewWidth },
	mOrbitCameraParameter{15.0f,Utility::Deg2Rad(-90.0f),0.0f},
	mCameraForward{0.0f,0.0f,-1.0f}
{
	UpdateCamera();
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, bool isPreview) const
{
	// 현재는 Scene에 Sphere 하나만 있다고 가정
	unsigned int renderW = 0;
	unsigned int renderH = 0;
	if (isPreview)
	{
		renderW = previewWidth;
		renderH = previewHeight;
	}
	else
	{
		renderW = width;
		renderH = height;
	}

	sf::Clock clock;
	for (size_t j = 0; j < renderH; j++) {
		for (size_t i = 0; i < renderW; i++) {
			frameBuffer[i + j * renderW] = Vec3f(j / float(renderH), i / float(renderW), 0);
			float x = (2 * (i + 0.5f) / (float)renderW - 1) * tan(fov / 2.0f)*renderW / (float)renderH;
			float y = -(2 * (j + 0.5f) / (float)renderH - 1) * tan(fov / 2.0f);
			Vec3f dir = ((mCameraRight * x) + (mCameraUp * y) + mCameraForward).normalize();
			Ray ray{ mCameraPosition, dir };
			frameBuffer[i + j * renderW] = CastRay(ray, isPreview, 0); //Depth 0부터 시작
		}
	}
	sf::Int32 elapsedTime = clock.getElapsedTime().asMilliseconds();
	return elapsedTime;
}

bool Renderer::EditCamera()
{
	bool e1 = ImGui::DragFloat("Camera r", &this->mOrbitCameraParameter.x, 0.01f, 0.0f, 25.0f);
	//ImGui::SameLine();
	bool e2 = ImGui::DragFloat("Camera yaw (rad)", &this->mOrbitCameraParameter.y, 0.01f, Utility::Deg2Rad(-180.0f), Utility::Deg2Rad(180.0f));
	bool e3 = ImGui::DragFloat("Camera pitch (rad)", &this->mOrbitCameraParameter.z, 0.01f, Utility::Deg2Rad(-89.0f), Utility::Deg2Rad(89.0f));
	bool e4 = ImGui::DragFloat("Camera FOV", &this->fov, 0.01f, Utility::Deg2Rad(10.0f), Utility::Deg2Rad(150.0f));
	
	UpdateCamera();

	return e1 | e2 | e3;
}

bool Renderer::EditorGUI()
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
	bool e2 = EditCamera();
	ImGui::DragInt("Max Depth", (int*)&maxDepth, 1.0f, 0, 10);
	ImGui::End();

	return e1 | e2;
}

void Renderer::SetScene(SceneManager* const scene)
{
	mScene = scene;
}

void Renderer::UpdateCamera()
{
	mCameraForward.x = cosf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward.y = sinf(mOrbitCameraParameter.z);
	mCameraForward.z = sinf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward = mCameraForward.normalize();

	mCameraPosition = -mOrbitCameraParameter.x * mCameraForward;

	mCameraRight = cross(mCameraForward, Vec3f{ 0.0f,1.0f,0.0f }).normalize();
	mCameraUp = cross(mCameraRight, mCameraForward).normalize();
}

Vec3f Renderer::CastRay(const Ray& ray, bool isPreview, size_t currentDepth) const
{
	Hit hit;

	if (isPreview)
	{
		if (currentDepth > 1 || !SceneIntersect(ray, hit))
		{
			return mScene->GetEnvironmentColor(ray.direction);
		}
	}
	else
	{
		if (currentDepth > maxDepth || !SceneIntersect(ray, hit))
		{
			return mScene->GetEnvironmentColor(ray.direction);
		}
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