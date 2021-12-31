#pragma once
#include "Camera.h"
#include "Hittable.h"
#include "FrameBuffer.h"

class RenderSettings{
public:
	RenderSettings() {}
	RenderSettings(unsigned int width, unsigned int height, unsigned int samples, unsigned int max_depth, Camera cam, hittable* world,  FrameBuffer* framebuffer) {
		this->width = width;
		this->height = height;
		this->samples = samples;
		this->max_depth = max_depth;
		this->cam = cam;
		this->world = world;
		this->framebuffer = framebuffer;
	}

	unsigned int width, height;
	unsigned int samples;
	unsigned int max_depth;
	Camera cam;
	hittable* world;
	FrameBuffer* framebuffer;
};