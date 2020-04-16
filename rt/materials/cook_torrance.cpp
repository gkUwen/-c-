#include <rt/materials/cook_torrance.h>
#include <rt/materials/material.h>
#include <core/scalar.h>
#include <math.h>

namespace rt
{
    CookTorranceMaterial::CookTorranceMaterial(Texture* texture, float diffuseCoeff,  float fresnelCoeff, float roughnessM)
    {
        this->texture = texture;
        this->fresnelCoeff = fresnelCoeff;
        this->diffuseCoeff = diffuseCoeff;
        this->roughnessM = roughnessM;
    }

    RGBColor CookTorranceMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const
    {
        // Idea inspired by: https://wiki.jmonkeyengine.org/jme3/advanced/pbr_part2.html#specular-brdf-cook-torrance an several google hack sources
        
        RGBColor diffuseColor = diffuseCoeff * this->texture->getColor(texPoint) * dot(-inDir, normal) / pi;
        
        Vector normalN = normal.normalize();
        Vector halfVector = (-inDir + outDir).normalize();
        float NdotO = std::max(dot(normalN, outDir), 0.f);
        float NdotI = std::max(dot(normalN, -inDir), 0.f);
        
        float specularCoeff = 0.0f;
        if(NdotI > 0.0f)
        {
            float IdotH = std::max(dot(-inDir, halfVector), 0.f);
            float OdotH = std::max(dot(outDir, halfVector), 0.f);
            float NdotH = std::max(dot(normalN, halfVector), 0.f);

			float F = this->fresnelCoeff + (1 - this->fresnelCoeff)*powf((1 - OdotH), 5);// Schlick approximation/Fresnel equation

            float roughnessMSq = this->roughnessM*this->roughnessM;
            float NdotHSq = NdotH*NdotH;
            float r1 =  1.0f/(4.0f*roughnessMSq*NdotHSq*NdotHSq);
            float r2 = (NdotHSq - 1 )/(roughnessMSq * NdotHSq);
            float D = r1*exp(r2);// microfacet Distribution function

            float Gb = 2*NdotH*NdotO/OdotH;
            float Gc = 2*NdotH*NdotI/IdotH;
            float G = std::min(1.0f, std::min(Gb, Gc));
            specularCoeff = F * D * G / pi * NdotI * NdotO;

        }

        RGBColor specularColor = (1 - this->diffuseCoeff)*texture->getColor(texPoint)*dot(-inDir, normal) * specularCoeff;

        RGBColor color = diffuseColor + specularColor;

        return color;
    }

    RGBColor CookTorranceMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const
    {
        return RGBColor(0.0f, 0.0f, 0.0f);
    }

    Material::SampleReflectance CookTorranceMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const
    {  
        return SampleReflectance();
    }

    Material::Sampling CookTorranceMaterial::useSampling() const
    {
        return Material::Sampling::SAMPLING_NOT_NEEDED;
    }

    Point CookTorranceMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}

}