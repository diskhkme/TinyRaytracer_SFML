#pragma once

#include "Geometry.h"

class Material
{
private:
	Vec4f albedo;
	Vec3f diffuseColor;
	float specularExponent;
	float refractiveIndex;

public:
	Material(const Vec4f& a, const Vec3f& color, const float spec, const float r)
		: albedo{ a }, diffuseColor{ color }, specularExponent{ spec }, 
		refractiveIndex{ r }
	{

	}

	Material()
		: albedo{ 1.0f, 0.0f, 0.0f, 0.0f }, diffuseColor{ 1.0f, 1.0f, 0.0f }, specularExponent{ 1.0f },
		refractiveIndex{ 1.0f }
	{

	}

	void SetDiffuseColor(const Vec3f color)
	{
		diffuseColor = color;
	}

	inline float GetSpecularExponent() const { return specularExponent; }
	inline const Vec3f& GetDiffuseColor() const { return diffuseColor; }
	inline const Vec4f& GetAlbedo() const { return albedo; }
	inline float GetRefractiveIndex() const { return refractiveIndex; }
};
