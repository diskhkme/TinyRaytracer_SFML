#pragma once

#include "Geometry.h"

class Material
{
private:
	Vec2f albedo;
	Vec3f diffuseColor;
	float specularExponent;

public:
	Material(const Vec2f& a, const Vec3f& color, const float spec)
		: albedo{ a }, diffuseColor{ color }, specularExponent{ spec }
	{

	}

	Material()
		: albedo{ 1.0f, 0.0f }, diffuseColor{ 1.0f, 1.0f, 0.0f }, specularExponent{ 1.0f }
	{

	}

	inline float GetSpecularExponent() const { return specularExponent; }
	inline const Vec3f& GetDiffuseColor() const { return diffuseColor; }
	inline const Vec2f& GetAlbedo() const { return albedo; }
};
