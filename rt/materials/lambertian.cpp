#include <rt/materials/lambertian.h>
#include <core/vector.h>
#include <core/scalar.h>
#include <core/point.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse)
{
    this->emission = emission;
    this->diffuse  = diffuse;
}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float cos_theta = fabs(dot(normal.normalize(), -inDir));// / (normal.length() * inDir.length());
    // float cos_theta = dot(normal.normalize(), inDir);
    RGBColor r = this->diffuse->getColor(texPoint)*cos_theta/pi;
    return r;
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    auto r = this->emission->getColor(texPoint);

    return r;
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return SampleReflectance(Vector(), RGBColor::rep(0.0f));
}

Material::Sampling LambertianMaterial::useSampling() const {
	return Sampling::SAMPLING_NOT_NEEDED;
}


Point LambertianMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}
}