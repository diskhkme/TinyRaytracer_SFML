#include "Camera.h"
#include "../Utility.h"

Camera::Camera(float fov)
	: mFov{ fov }, 
	mOrbitCameraParameter{ 15.0f,Utility::Deg2Rad(-90.0f),0.0f },
	mCameraForward{ 0.0f,0.0f,-1.0f }
{
	UpdateCamera();

	mLowerLeftCorner = mCameraPosition - (mCameraRight * 0.5f) - (mCameraUp * 0.5f) + mCameraForward;
}

Camera::Camera()
	: Camera{ 3.14f / 3.0f }
{}

void Camera::UpdateCamera()
{
	mCameraForward.x = cosf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward.y = sinf(mOrbitCameraParameter.z);
	mCameraForward.z = sinf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward = mCameraForward.normalize();

	mCameraPosition = -mOrbitCameraParameter.x * mCameraForward;

	mCameraRight = cross(mCameraForward, Vec3f{ 0.0f,1.0f,0.0f }).normalize();
	mCameraUp = cross(mCameraRight, mCameraForward).normalize();
}

Ray Camera::GetRay(size_t i, size_t j, size_t renderWidth, size_t renderHeight)
{
	float x = (2 * (i + 0.5f) / (float)renderWidth - 1) * tan(mFov / 2.0f)*renderWidth / (float)renderHeight;
	float y = -(2 * (j + 0.5f) / (float)renderHeight - 1) * tan(mFov / 2.0f);
	Vec3f dir = ((mCameraRight * x) + (mCameraUp * y) + mCameraForward).normalize();
	return Ray{ mCameraPosition, dir };
}

bool Camera::EditCamera()
{
	bool e1 = ImGui::DragFloat("Camera r", &this->mOrbitCameraParameter.x, 0.01f, 0.0f, 25.0f);
	//ImGui::SameLine();
	bool e2 = ImGui::DragFloat("Camera yaw (rad)", &this->mOrbitCameraParameter.y, 0.01f, Utility::Deg2Rad(-180.0f), Utility::Deg2Rad(180.0f));
	bool e3 = ImGui::DragFloat("Camera pitch (rad)", &this->mOrbitCameraParameter.z, 0.01f, Utility::Deg2Rad(-89.0f), Utility::Deg2Rad(89.0f));
	bool e4 = ImGui::DragFloat("Camera FOV", &this->mFov, 0.01f, Utility::Deg2Rad(10.0f), Utility::Deg2Rad(150.0f));

	UpdateCamera();

	return e1 | e2 | e3;
}