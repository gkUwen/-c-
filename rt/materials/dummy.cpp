#include <rt/materials/dummy.h>
#include <core/assert.h>
#include <math.h>

namespace rt {

DummyMaterial::DummyMaterial() {
    
}

RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    
    return RGBColor::rep(fabs(dot(normal.normalize(), inDir.normalize())));
}

RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    /* TODO */ NOT_IMPLEMENTED;
}

Point DummyMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}
}