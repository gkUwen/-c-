#include <rt/materials/combine.h>
#include <rt/materials/material.h>

namespace rt {

CombineMaterial::CombineMaterial()
{
    /* TODO */
}

void CombineMaterial::add(Material* m, float w) {
    mats.push_back(std::make_pair(m, w));
    Material::Sampling currentSampling = m->useSampling();
    if(getSamplingPriority(currentSampling) > getSamplingPriority(sampling))
    {
        sampling = currentSampling;
    }
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
    
    for(int i = 0; i < mats.size(); i++)
    {
        if (mats[i].first->useSampling() == Material::SAMPLING_NOT_NEEDED) 
        {
            color = color + mats[i].second * mats[i].first->getReflectance(texPoint, normal, outDir, inDir);
        }
    }
    return color;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < mats.size(); ++i)
    {
        color = color + mats[i].second * mats[i].first->getEmission(texPoint, normal, outDir);
    }
    return color;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    Material::SampleReflectance sampleReflectance;
    
    for (int i = 0; i < mats.size(); ++i) 
    {
        if (mats[i].first->useSampling() == Material::SAMPLING_ALL)
        {
            sampleReflectance = mats[i].first->getSampleReflectance(texPoint, normal, outDir);
            sampleReflectance.reflectance = mats[i].second * sampleReflectance.reflectance;
            break;
        }    
    }
    return sampleReflectance;
}

Material::Sampling CombineMaterial::useSampling() const {
    return Material::SAMPLING_SECONDARY;
}

int getSamplingPriority(Material::Sampling samplingType)  
{
    if(samplingType == Material::SAMPLING_NOT_NEEDED) return 0;
    else if(samplingType == Material::SAMPLING_SECONDARY) return 1;
    else if(samplingType == Material::SAMPLING_ALL) return 2;
}

Point CombineMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}

}