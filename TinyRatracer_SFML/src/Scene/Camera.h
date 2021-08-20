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

	Ray GetRay(dist_t u, dist_t v, dist_t aspectRatio);
	bool EditCamera();
	void UpdateCamera();
};