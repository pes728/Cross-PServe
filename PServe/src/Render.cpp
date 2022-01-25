//
//  Render.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/28/20.
//

#include "Render.h"


Scene spheres() {
    Scene s;

    auto checker = make_shared<checker_texture>(glm::vec3(0.2, 0.3, 0.1), glm::vec3(0.9, 0.9, 0.9));

    auto ground_material = make_shared<lambertian>(checker);
    s.spheres.push_back(Sphere(glm::vec3(0, -1000, 0), 1000, ground_material));

    auto material1 = make_shared<dielectric>(1.5);
    s.spheres.push_back(Sphere(glm::vec3(0, 1, 0), 1.0, material1));

    auto solid = make_shared<solid_color>(glm::vec3(random_float()));
    auto material2 = make_shared<lambertian>(solid);
    s.spheres.push_back(Sphere(glm::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
    s.spheres.push_back(Sphere(glm::vec3(4, 1, 0), 1.0, material3));

    return s;
}

hittable_list one_sphere(){
    hittable_list world;

    auto checker = make_shared<checker_texture>(glm::vec3(0.2, 0.3, 0.1), glm::vec3(0.9, 0.9, 0.9));

    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<Sphere>(glm::vec3(0,-1000,0), 1000, ground_material));
    
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(glm::vec3(0, 1, 0), 1.0, material1));

    auto solid = make_shared<solid_color>(glm::vec3(random_float()));
    auto material2 = make_shared<lambertian>(solid);
    world.add(make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material3));
    


    return world;
}

glm::vec3 Background(const ray& r) {
    glm::vec3 unit_direction = glm::normalize(r.direction());
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}

Frame ray_colorR(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    Frame frame;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) {
        frame.color = glm::vec3(0, 0, 0);
        frame.normal = glm::vec3(0, 0, 0);
        return frame;
    }

    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec)) {
        frame.color = frame.albedo = Background(r);
        frame.normal = glm::normalize(-r.dir);
        return frame;
    }

    ray scattered;
    glm::vec3 attenuation;
    
    if (!rec.mat->scatter(r, rec, attenuation, scattered)) {
        frame.color = glm::vec3(0, 0, 0);
        frame.normal = rec.normal;
        return frame;
    }

    Frame f = ray_colorR(scattered, world, depth - 1);

    frame.normal = rec.normal;
    frame.albedo = attenuation;
    frame.color = attenuation * f.color;
    return frame;
}

/*Frame ray_colorI(const ray& r, const hittable& world, int max_depth) {
    vec3 color(1.0, 1.0, 1.0);

    ray cur_ray = r;

    bool anyhit = false;

    for (int i = 0; i < max_depth; i++) {
        hit_record rec;
        if (world.hit(cur_ray, 0.0001, infinity, rec)) {
            anyhit = true;
            ray scattered;
            vec3 attenuation;
            if (rec.mat->scatter(cur_ray, rec, attenuation, scattered)) {
                color *= attenuation;
                cur_ray = scattered;
            }
            else {
                color = vec3(0.0, 0.0, 0.0);
                break;
            }
        }
        else {
            vec3 unit_direction = unit_vector(cur_ray.direction());
            float t = 0.5f * (unit_direction.y() + 1.0f);
            vec3 c = (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
            color *= c;
        }
    }

    if (!anyhit) color = vec3(0.0, 0.0, 0.0);

    Frame frame;
    frame.color = color;
}*/



void render_pixel(int x, int y, RenderSettings settings){
    glm::vec3 color(0, 0, 0);
    glm::vec3 albedo(0, 0, 0);
    glm::vec3 normal(0, 0, 0);
    for (unsigned int s = 0; s < settings.samples; s++) {
        auto u = (x + random_float()) / (settings.width-1);
        auto v = (y + random_float()) / (settings.height-1);
        ray r = settings.cam.get_ray(u, v);


        Frame f = ray_colorR(r, *(settings.world), settings.max_depth);
        color += f.color;
        albedo += f.albedo;
        normal += f.normal;
    }

    setPixel(x, y, settings.width, settings.height, color / glm::vec3(settings.samples), settings.framebuffer->color, true);
    setPixel(x, y, settings.width, settings.height, albedo / glm::vec3(settings.samples), settings.framebuffer->albedo, true);
    setPixel(x, y, settings.width, settings.height, glm::normalize(normal / glm::vec3(settings.samples)), settings.framebuffer->normal, true);
}

void thread_manager(int id, int jump_value, RenderSettings settings){
    unsigned int index = id;
    while(index < settings.width * settings.height){
        render_pixel(index % settings.width , index / settings.width, settings);
        index += jump_value;
    }
}

void render(RenderSettings settings, std::atomic_bool* finished){
    std::clock_t start = std::clock();

    if (MULTI_THREADED) {
        std::vector<std::thread> renderThreads(THREADS);

        for (unsigned int i = 0; i < THREADS; i++) renderThreads[i] = std::thread(thread_manager, i, THREADS, settings);
        for (unsigned int i = 0; i < THREADS; i++) renderThreads[i].join();
        finished->store(true);
    }
    //SingleThreaded
    else {
        for (unsigned int y = 0; y < settings.height; y++) {
            std::cout << "Scanlines completed: " << y << '\r' << std::flush;
            for (unsigned int x = 0; x < settings.width; x++) {
                render_pixel(x, y, settings);
            }
        }
    }
    std::cout << "ALL DONE!" << std::endl;
    std::clock_t end = std::clock();
    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
    << (end-start) / CLOCKS_PER_SEC << " s\n";
}



bool Scene::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
    hit_record temp_rec;
    bool hitAnything = false;
    float closestHit = infinity;
    for (auto& sphere : spheres) {
        
        if (sphere.hit(r, tmin, closestHit, temp_rec)) {
            closestHit = temp_rec.t;
            hitAnything = true;
            rec = temp_rec;
        }
    }
    return hitAnything;
}