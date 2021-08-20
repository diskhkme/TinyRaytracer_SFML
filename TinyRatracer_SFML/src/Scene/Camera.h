#pragma once

#include "../Geometry.h"
#include "../Renderer/Ray.h"

#include "../../imgui-sfml/imgui.h"
#include "../../imgui-sfml/imgui-SFML.h"

class Camera 
{
private:
	float mFov;
	Vec3f mOrbitCameraParameter; // (x,y,z) => (r, yaw, pitch)
	Vec3f mCameraPosition;
	Vec3f mCameraForward;
	Vec3f mCameraRight;
	Vec3f mCameraUp;

	Vec3f mLowerLeftCorner;

public:
	Camera();
	Camera(float fov);

	Ray GetRay(size_t i, size_t j, size_t renderWidth, size_t renderHeight);
	bool EditCamera();
	void UpdateCamera();
};