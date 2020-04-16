#include <rt/integrators/raytrace.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = world->scene->intersect(ray);
    RGBColor color = RGBColor(0.f, 0.f, 0.f);

    if (intersection.if_intersect) {
        Vector normal = intersection.normal();
        Point texture = intersection.solid->texMapper->getCoords(intersection);

        if (dot(normal, ray.d) > 0) normal = -1.0f*normal;

        Ray shadow_ray;

        for (int i = 0; i < this->world->light.size(); i++){
            LightHit light_hit = this->world->light[i]->getLightHit(intersection.hitPoint());

            if (dot(normal, light_hit.direction) < 0) continue;

            shadow_ray.o = intersection.hitPoint() + 1e-5 * light_hit.direction; //offset ray gap tolerance
            shadow_ray.d = light_hit.direction;
            Intersection shadow_hit = world->scene->intersect(shadow_ray, light_hit.distance);
            
            if (!shadow_hit) {
                RGBColor reflectance = intersection.solid->material->getReflectance(texture, normal, -ray.d, shadow_ray.d);
                color = color + reflectance * this->world->light[i]->getIntensity(light_hit);
            }
        }
        RGBColor emission = intersection.solid->material->getEmission(texture, intersection.normal(), -ray.d);
        
        color = color + emission;
    }
    return color;
}

}
