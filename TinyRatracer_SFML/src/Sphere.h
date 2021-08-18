﻿#pragma once

#include "Geometry.h"
#include "Material.h"

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

class Sphere
{
private:
	Vec3f center;
	float radius;
	Material material;

public:
	Sphere(const Vec3f& c, const float& r, const Material& mat) : center{ c }, radius{ r }, material{ mat }
	{

	}

	bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& closest) const
	{
		Vec3f L = center - orig; //orig에서 출발하여 center를 향하는 벡터
		float tca = L * dir; // L vector를 dir vector에 projection
		float d2 = L * L - tca * tca; //L^2 = tca^2 + r^2인 경우 접하는 삼각형.
		if (d2 > radius*radius) return false; //r^2보다 거리가 멀면, ray가 원에 부H히지 않음
		float thc = sqrtf(radius*radius - d2);
		float t0 = tca - thc; //t0는 현재 구에 대한 t
		float t1 = tca + thc; //t0와 t1은 가까운/먼 교차점까지의 파라메터
		if (t0 < 0) t0 = t1;  //접점이 하나인 경우 or dir 반대 방향에 원이 있는 경우를 처리
		if (t0 < 0) return false;
		
		if (t0 < closest) //현재 구가 가장 가까운 구인지 확인
		{
			closest = t0;
			return true;
		}
		return false;
	}

	bool EditSphere()
	{
		bool e1 = material.EditMaterial();
		bool e2 = ImGui::DragFloat3("sphere position", &this->center.x);
		bool e3 = ImGui::SliderFloat("sphere radius", &this->radius,0.1f, 10.0f);
		return e1 | e2 | e3;
	}
	
	inline Vec3f GetCenter() const { return center;	}
	inline Material GetMaterial() const { return material; }
};