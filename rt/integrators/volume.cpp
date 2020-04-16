#include <rt/integrators/raytrace.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/integrators/volume.h>


/*question 1*/
namespace rt {

RGBColor volume::getRadiance(const Ray& ray) const {
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
        RGBColor fog = RGBColor(1, 1, 1);
        float fog_exp = expf(-0.2*fabsf(intersection.distance));
        color = fog_exp*color+(1-fog_exp)*fog;
    }
    return color;
}

}

/*question 2*/
namespace rt {

// RGBColor volume::getRadiance(const Ray& ray) const {
//     Intersection intersection = world->scene->intersect(ray);
//     RGBColor sum_color = RGBColor(0.f, 0.f, 0.f);
   
//     float mu = 0.01;
//     float g = 0;

//     if (intersection.if_intersect) {
//         float hitray_length = intersection.distance;
//         std::vector<Point> march_pts;
//         float curr_length = 0.0f;
//         int step = 10;
//         float march_step = hitray_length / step;
//         for(int i=0;i<step-1;i++)
//         {
//             curr_length += march_step;
//             march_pts.push_back(ray.o+curr_length*ray.d.normalize());
//         }
       
//         RGBColor hitray_attenuate = RGBColor::rep(1.0f);
//         for(int j=0; j<march_pts.size(); j++) 
//         {
//             RGBColor shodow_color = RGBColor::rep(0.0f);
//             for (int i = 0; i < this->world->light.size(); i++) 
//             {
//                 LightHit light_hit = this->world->light[i]->getLightHit(march_pts[j]);
//                 if (dot(intersection.normal(), light_hit.direction) < 0) continue;
//                 Ray shadow_ray;
//                 shadow_ray.o = march_pts[j] + 1e-5 * light_hit.direction; //offset ray gap tolerance
//                 shadow_ray.d = light_hit.direction;
//                 Intersection shadow_hit = world->scene->intersect(shadow_ray, light_hit.distance);

//                 if (!shadow_hit.if_intersect) 
//                 {
//                     RGBColor shadow_attenuate = this->world->light[i]->getIntensity(light_hit)*exp(-light_hit.distance*mu);
//                     float fp = 1 / (4 * pi)*(1 - g*g) / sqrtf(pow((1 + g*g + 2 * g*dot(-ray.d, -light_hit.direction)), 3));
//                     shodow_color = shodow_color + shadow_attenuate*fp;
//                 }
//             }
//             hitray_attenuate = hitray_attenuate* exp(-march_step*mu);
//             sum_color = sum_color + shodow_color*hitray_attenuate*march_step;
//         }


//         Vector normal = intersection.normal();
//         Point texture = intersection.solid->texMapper->getCoords(intersection);

//         if (dot(normal, ray.d) > 0) normal = -1.0f*normal;

//         Ray shadow_ray;
      
//         for (int i = 0; i < this->world->light.size(); i++){
//             LightHit light_hit = this->world->light[i]->getLightHit(intersection.hitPoint());

//             if (dot(normal, light_hit.direction) < 0) continue;

//             shadow_ray.o = intersection.hitPoint() + 1e-5 * light_hit.direction; //offset ray gap tolerance
//             shadow_ray.d = light_hit.direction;
//             Intersection shadow_hit = world->scene->intersect(shadow_ray, light_hit.distance);
            
//             if (!shadow_hit) {
//                 RGBColor reflectance = intersection.solid->material->getReflectance(texture, normal, -ray.d, shadow_ray.d);
//                 sum_color = sum_color + reflectance * this->world->light[i]->getIntensity(light_hit)*exp(-light_hit.distance*mu);
//             }
//         }
//         RGBColor emission = intersection.solid->material->getEmission(texture, intersection.normal(), -ray.d);
//         sum_color = sum_color + emission;
  
//         sum_color = sum_color + sum_color*exp(-hitray_length*mu);
//     }
//     return sum_color;
// }

}
