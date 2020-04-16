#include <rt/materials/phong.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
{
    this->specular = specular;
    this->exponent = exponent;
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    
    float I_dot_N = dot(normal.normalize(), -inDir.normalize());
    Vector R = inDir + (2 * I_dot_N*normal.normalize());
    float cos_theta = dot(R, outDir.normalize());
    RGBColor ks = this->specular->getColor(texPoint);

    return (ks * max(0.f, powf(cos_theta,this->exponent))) * I_dot_N * exponent / (2*pi);
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return SampleReflectance(Vector(), RGBColor::rep(0.f));
}

Material::Sampling PhongMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

Point PhongMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}

}