#include <rt/lights/arealight.h>

#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
	LightHit hit;
    auto ls_point = source->sample();
	Vector dir = ls_point.point - p;
	hit.direction = dir.normalize();
	hit.distance = dir.length() - 1e-5f;
	hit.normal = ls_point.normal;

	return hit;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
	float s = dot(irr.direction, irr.normal);
	s = s >= 0 ? s : -s;
	return source->getArea() * source->material->getEmission(Point::rep(0), Vector::rep(0), Vector::rep(0)) * s / (irr.distance * irr.distance);
}

AreaLight::AreaLight(Solid* source)
	:source(source)
{
}

}