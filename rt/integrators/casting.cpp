#include <rt/integrators/casting.h>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersect = this->world->scene->intersect(ray);
    float value = 0.0f;
    RGBColor color(value, value, value);
    if(intersect.if_intersect)
    {
        value = (-1.0f* dot(intersect.normal(), ray.d));
        color = RGBColor(value, value, value).clamp();
    }
    return color;
}

}
