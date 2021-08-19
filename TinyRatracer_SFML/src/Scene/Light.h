#pragma once

#include "../Geometry.h"

#include "../../imgui-sfml/imgui.h"
#include "../../imgui-sfml/imgui-SFML.h"

class Light
{
private:
	Vec3f position;
	float intensity;

public:
	//point light
	Light(const Vec3f& p, const float& i)
		: position{ p }, intensity{ i }
	{

	}

	inline Vec3f GetPosition() const { return position; }
	inline float GetIntensity() const { return intensity; }

	bool EditLight()
	{
		bool e1 = ImGui::DragFloat3("position", &this->position.x);
		bool e2 = ImGui::SliderFloat("intensity", &this->intensity, 0.1f, 10.0f);
		return e1 | e2;
	}
};