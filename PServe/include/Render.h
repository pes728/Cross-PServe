//
//  Render.h
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//



#ifndef Render_h
#define Render_h

#define MULTI_THREADED 1

#ifdef MULTI_THREADED
#define THREADS 12
#endif

#include <thread>
#include "Camera.h"
#include "bvh_node.h"
#include "hittable_list.h"
#include "Image.h"
#include <iostream>
#include "Sphere.h"
#include "moving_sphere.h"
#include "Material.h"
#include <ctime>
#include "FrameBuffer.h"
#include <iomanip>
#include "RenderSettings.h"
#include <vector>

class Scene{
public:
	bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	std::vector<Sphere> spheres;
};

void render(RenderSettings, std::atomic_bool* finished);

hittable_list one_sphere();

Scene spheres();

void render_pixel(int x, int y, int image_width, int image_height, int samples, int max_depth, Camera c, const hittable& world, FrameBuffer* frame);

Frame ray_colorR(const ray& r, const Scene& world, int max_depth);

//Frame ray_colorI(const ray& r, const hittable& world, int max_depth);

#endif /* Render_hpp */
