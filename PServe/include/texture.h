#pragma once
#include "common.h"

class texture {
public:
	virtual glm::vec3 value(float u, float v, const glm::vec3& point) const = 0;
};


class solid_color : public texture{
public:
	solid_color() {}
	solid_color(glm::vec3 color) : color_value(color) {}
	solid_color(float red, float green, float blue) {
		color_value = glm::vec3(red, green, blue);
	}

	virtual glm::vec3 value(float u, float v, const glm::vec3& point) const override {
		return color_value;
	}

	glm::vec3 color_value;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(shared_ptr<texture> odd, shared_ptr<texture> even) : odd(odd), even(even){}

	checker_texture(glm::vec3 odd, glm::vec3 even) : odd(make_shared<solid_color>(odd)), even(make_shared<solid_color>(even)){}

	virtual glm::vec3 value(float u, float v, const glm::vec3& point) const override {
		auto sines = sin(10 * point[0]) * sin(10 * point[1]) * sin(10 * point[2]);
		if (sines < 0) return odd->value(u, v, point);
		else return even->value(u, v, point);
	}

	shared_ptr<texture> odd, even;
};
