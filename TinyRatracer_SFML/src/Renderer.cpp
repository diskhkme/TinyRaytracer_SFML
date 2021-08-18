#include "Renderer.h"
#include "Utility.h"

#include <algorithm>

Renderer::Renderer(unsigned int w, unsigned int h, float fov,
	unsigned int previewWidth, unsigned int previewHeight)
	:width{ w }, height{ h }, fov{ fov }, 
	previewHeight{previewHeight}, previewWidth { previewWidth },
	mOrbitCameraParameter{11.0f,Utility::Deg2Rad(-90.0f),0.0f},
	mCameraForward{0.0f,0.0f,-1.0f}
{
	UpdateCamera();
}

sf::Int32 Renderer::Render(std::vector<Vec3f>& frameBuffer, const std::vector<Sphere> & scene, 
	const std::vector<Light>& lights, bool isPreview) const
{
	// ����� Scene�� Sphere �ϳ��� �ִٰ� ����
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
			frameBuffer[i + j * renderW] = CastRay(mCameraPosition, dir, scene, lights); //ī�޶�� mCameraPosition�� ��ġ
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

Vec3f Renderer::CastRay(const Vec3f & origin, const Vec3f & direction, const std::vector<Sphere> & scene, 
	const std::vector<Light>& lights) const
{
	Vec3f hit, normal;
	Material material;

	if (!SceneIntersect(origin, direction, scene, hit, normal, material))
	{
		return Vec3f{ 0.2f, 0.7f, 0.8f };
	}
	// �����ϴ� ��ü�� �ִ°�� hit,normal,material ������ �����

	float diffuseIntensity = 0;
	float specularIntensity = 0;

	for (const Light& light : lights)
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

bool Renderer::SceneIntersect(const Vec3f & origin, const Vec3f direction, const std::vector<Sphere>& scene,
	Vec3f & hit, Vec3f & normal, Material & material) const
{
	float sphereDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;
	for (const Sphere& s : scene)
	{
		if (s.RayIntersect(origin, direction, sphereDist))
		{
			hit = origin + direction * sphereDist;
			normal = (hit - s.GetCenter()).normalize();
			material = s.GetMaterial();
		}
	}

	return sphereDist < 1000; // �ִ� �Ÿ�. Camera�� far plane�̸� �����ϰ���?
}

Vec3f Renderer::Reflect(const Vec3f & l, const Vec3f & n) const
{
	return l - n * 2.0f*(n*l);
}
