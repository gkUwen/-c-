#include <rt/lights/pointlight.h>

#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
    this->intensity = intensity;
    this->position = position;
}

LightHit PointLight::getLightHit(const Point& p) const {
    LightHit hit_light;
    hit_light.direction = Vector(this->position - p).normalize();
    hit_light.distance = Vector(this->position - p).length();
    hit_light.normal = hit_light.direction;// Vector(this->position - p).normalize();

    return hit_light;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return RGBColor(this->intensity / (irr.distance*irr.distance));
}

}
