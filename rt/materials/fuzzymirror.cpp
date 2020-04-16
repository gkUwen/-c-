#include <rt/materials/fuzzymirror.h>
#include <rt/materials/material.h>
#include <core/scalar.h>
#include <core/random.h>

namespace rt {

FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle)
{
    this->eta = eta;
	this->kappa = kappa;
	this->fuzzyangle = fuzzyangle;
}

RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	return RGBColor::rep(0.f);
}

RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
   return RGBColor::rep(0.f);
}

Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    float theta = dot(outDir, normal);
	Vector reflect = (-outDir + 2 * theta * normal).normalize();

	float parallel = ( (this->eta*this->eta + this->kappa*this->kappa)*theta*theta - 2*this->eta*theta + 1 ) 
	                 / ( (this->eta*this->eta + this->kappa*this->kappa)*theta*theta + 2*this->eta*theta + 1 );

    float perpendicular = ( (this->eta*this->eta + this->kappa*this->kappa)- 2*this->eta*theta + theta*theta ) 
	                      / ( (this->eta*this->eta + this->kappa*this->kappa) + 2*this->eta*theta + theta*theta );
    
	Vector x_sample, y_sample;
	if(theta!=1.f)
	{
       x_sample = cross(outDir,reflect).normalize();
	}
	else
	{
		x_sample = cross(Vector(0.5,0.5,0.5),reflect).normalize();
	}
	y_sample = cross(x_sample,reflect).normalize();

	float radius = tanf(fuzzyangle);
	float angle = 2*pi*random();

	reflect = reflect + x_sample*cos(angle)*radius + y_sample*sin(angle)*radius;
	return Material::SampleReflectance(reflect.normalize(), RGBColor::rep(0.5*(parallel*parallel + perpendicular*perpendicular)));
}

Material::Sampling FuzzyMirrorMaterial::useSampling() const {
   return Sampling::SAMPLING_ALL;
}

Point FuzzyMirrorMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}
}



