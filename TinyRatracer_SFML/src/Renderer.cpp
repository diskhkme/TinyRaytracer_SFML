#include "Renderer.h"
#include "Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, float fov, size_t maxDepth,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, maxDepth{ maxDepth },
	previewHeight{previewHeight}, previewWidth { previewWidth },
	mOrbitCameraParameter{11.0f,Utility::Deg2Rad(-90.0f),0.0f},
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
			frameBuffer[i + j * renderW] = CastRay(mCameraPosition, dir, 0); //Depth 0부터 시작
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
	bool e1 = mScene.EditScene();
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

void Renderer::SetScene(const SceneManager & scene)
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

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, size_t currentDepth) const
{
	Vec3f hit, normal;
	Material material;

	if (currentDepth > maxDepth || !SceneIntersect(origin, direction, hit, normal, material))
	{
		return Vec3f{ 0.2f, 0.7f, 0.8f };
	}
	// 교차하는 물체가 있는경우 hit,normal,material 정보가 저장됨

	//---Reflection
	Vec3f reflectDir = Reflect(direction, normal).normalize();
	Vec3f reflectOrigin = reflectDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	Vec3f reflectColor = CastRay(reflectOrigin, reflectDir, currentDepth + 1);

	//---Refraction
	Vec3f refractDir = Refract(direction, normal, material.GetRefractiveIndex()).normalize();
	Vec3f refractOrigin = refractDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	Vec3f refractColor = CastRay(refractOrigin, refractDir, currentDepth + 1);

	float diffuseIntensity = 0;
	float specularIntensity = 0;

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
	Vec3f & hit, Vec3f & normal, Material & material) const
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;
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