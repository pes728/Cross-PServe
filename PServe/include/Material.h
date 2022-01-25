//
//  Material.h
//  PRender
//
//  Created by Peter Sandquist on 9/29/20.
//

#ifndef Material_h
#define Material_h
#include "texture.h"

struct hit_record;


inline double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}


class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};


class lambertian : public material {
public:
    lambertian(const glm::vec3& color) : albedo(make_shared<solid_color>(color)) {}
    lambertian(shared_ptr<texture> a) : albedo(a) {}
    
    virtual bool scatter(
                         const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered
                         ) const override {
        glm::vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    
public:
    shared_ptr<texture> albedo;
};


class metal : public material {
public:
    metal(const glm::vec3& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
    virtual bool scatter(
                         const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered
                         ) const override {
        glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
public:
    glm::vec3 albedo;
    float fuzz;
};


class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered
        ) const override {
            attenuation = glm::vec3(1.0, 1.0, 1.0);
            float refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            glm::vec3 unit_direction = glm::normalize(r_in.direction());
            double cos_theta = fmin(glm::dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = glm::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            glm::vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction, r_in.time());
            return true;
        }

    public:
        float ir; // Index of Refraction

    private:
        static float reflectance(float cosine, float ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif /* Material_h */
