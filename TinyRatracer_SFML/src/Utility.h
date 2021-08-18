#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Geometry.h"

#define MY_PI (3.1415917f)

class Utility {
public:
	inline static float Deg2Rad(float deg)
	{
		return deg * MY_PI / 180.0f;
	}

	inline static float Rad2Deg(float rad)
	{
		return rad * 180.0f / MY_PI;
	}

	inline static sf::Uint8 ConvertFloatToColor(float val)
	{
		return (val >= 0.0f) ? static_cast<sf::Uint8>(val*255.0f) : 0;
	}

	static void ConvertPixelsFromVector(const std::vector<Vec3f>& framebuffer, sf::Uint8* pixels)
	{
		size_t pixelCount = framebuffer.size();

		for (size_t i = 0; i < pixelCount; i++)
		{
			pixels[4 * i + 0] = ConvertFloatToColor(framebuffer[i].x);
			pixels[4 * i + 1] = ConvertFloatToColor(framebuffer[i].y);
			pixels[4 * i + 2] = ConvertFloatToColor(framebuffer[i].z);
			pixels[4 * i + 3] = 255;
		}
	}

	static void SaturateColor(Vec3f& color)
	{
		color.x = std::min(color.x, 1.0f);
		color.y = std::min(color.y, 1.0f);
		color.z = std::min(color.z, 1.0f);
	}

};

