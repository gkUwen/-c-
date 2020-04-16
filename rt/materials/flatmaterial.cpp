#include <rt/materials/flatmaterial.h>
#include <core/point.h>

namespace rt {

FlatMaterial::FlatMaterial(Texture* texture)
{
    this->texture = texture;
}

RGBColor FlatMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

RGBColor FlatMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return this->texture->getColor(texPoint);
}

Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
}

Material::Sampling FlatMaterial::useSampling() const {
   return SAMPLING_NOT_NEEDED;
}


Point FlatMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}

}