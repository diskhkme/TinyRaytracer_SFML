#pragma once

#include "Material.h"
#include "../Renderer/Hit.h"

class ModelBase
{
protected:
	std::string name;
	Material material;

public:
	ModelBase(const std::string& name, const Material& m) : name{ name }, material { m } {}
	virtual bool RayIntersect(const Ray& ray, Hit& hit) const = 0;

	virtual bool EditModel() = 0;
public:
	inline const Material& GetMaterial() const { return material; }
	inline const std::string& GetName() const { return name; }
};