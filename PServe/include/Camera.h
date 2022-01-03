//
//  Camera.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef Camera_h
#define Camera_h

#include "common.h"
#include "ray.h"

class Camera{
public:
    Camera() {

    }
    Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist, float t0, float t1){
        float theta = vfov * pi / 180.0f;
        float h = tan(theta/2);
        float viewport_height = 2.0f * h;
        float viewport_width = aspect_ratio * viewport_height;

        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2.0f - vertical/2.0f - focus_dist*w;

        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;
    }
    
    ray get_ray(float s, float t) const{
        glm::vec3 rd = lens_radius * random_in_unit_disk();
        glm::vec3 offset = u * rd[0] + v * rd[1];
        return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset, random_float(time0, time1));
    }
    
    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lens_radius;
    float time0, time1;
};

#endif /* Camera_h */
