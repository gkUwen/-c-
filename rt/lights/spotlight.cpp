#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
    this->p_light = new PointLight(position, intensity);
    this->direction = direction;
    this->angle = angle;
    this->power = power;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const 
{
    float coff = 0.0;
    float cos_angle_current = dot(this->direction.normalize(), -irr.direction.normalize());
    RGBColor intensity= this->p_light->getIntensity(irr);
    
    if(acos(cos_angle_current) < this->angle)
    {   
        coff = powf(cos_angle_current, this->power);
    }
    return intensity*coff;
}

LightHit SpotLight::getLightHit(const Point& p) const
{
    return this->p_light->getLightHit(p);
}


}

