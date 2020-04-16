#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
    this->direction = direction;
    this->intensity = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    LightHit hit_light;
    hit_light.distance = FLT_MAX;
    hit_light.direction = -(this->direction);
    return  hit_light;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return this->intensity;
}

}
