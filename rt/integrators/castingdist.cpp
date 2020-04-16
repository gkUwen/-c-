#include <rt/integrators/castingdist.h>
namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    this->nearColor = nearColor;
    this->nearDist  = nearDist;
    this->farColor  = farColor;
    this->farDist   = farDist;
}
RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersect = this->world->scene->intersect(ray);
    RGBColor color(0.0, 0.0, 0.0);

    if(intersect.if_intersect){
 
        float nearDist_ = this->nearDist;
        float farDist_ = this->farDist;
        if (intersect.distance < this->nearDist) nearDist_ = intersect.distance;
        if (intersect.distance > this->farDist) farDist_ = intersect.distance;
        float ratio = (intersect.distance - nearDist_)/(farDist_ - nearDist_);
        RGBColor dist_color = (this->farColor - this->nearColor )*ratio + this->nearColor;
        float cos_ = dot(ray.d.normalize(), intersect.normal().normalize());//|u||v|cosa = dot(u,v) and |u||v|=1
        return (dist_color.clamp() * -1.0f*cos_);
    }

    return color; 
}
}
