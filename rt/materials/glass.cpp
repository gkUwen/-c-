#include <rt/materials/glass.h>
#include <core/scalar.h>
#include <rt/materials/material.h>
#include <core/scalar.h>
#include <core/random.h>

namespace rt {

GlassMaterial::GlassMaterial(float eta)
{
    this->eta = eta;
}


RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0.f);
}
RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.f);
}

Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
   //https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
   
   float theta1_cos = dot(outDir,normal);//theta1 is cos(angle) between input ray and normal, theta2 is cos(angle) between -normal and refracted ray 
   float curr_eta = this->eta;
  
   if(theta1_cos < 0)
   {
       curr_eta = 1/curr_eta;
   }
   
   Vector refle_dir = outDir - 2*theta1_cos*normal; 
   float theta1_sin = sin(acos(theta1_cos));
   float theta2_sin = theta1_sin / curr_eta;

   if(theta2_sin > 1)
   {
	   return Material::SampleReflectance(-refle_dir.normalize(), RGBColor::rep(1.0f)); 
   }
   
   float theta2_cos = cos(asin(theta2_sin));

   float parallel = ( (curr_eta*theta1_cos - theta2_cos) / (curr_eta*theta1_cos + theta2_cos) );
	        
   float perpendicular = ( (theta1_cos - curr_eta*theta2_cos) / (theta1_cos + curr_eta*theta2_cos) );
	                    
   float refle = 0.5*(parallel*parallel + perpendicular*perpendicular);
   

    if(random()>0.5)
    {
       return Material::SampleReflectance(-refle_dir.normalize(), RGBColor::rep(2*refle));
    }
    else
    {
        Vector M = (normal*theta1_cos-outDir).normalize();
        Vector N = -(normal*theta1_cos).normalize();
		Vector refra_dir = (N * theta2_cos) + (M * theta2_sin);

        return Material::SampleReflectance(refra_dir.normalize(), RGBColor::rep(2*(1-refle)) );
    }
}

Material::Sampling GlassMaterial::useSampling() const {
   return Sampling::SAMPLING_ALL;
}

Point GlassMaterial::getRayOrigin(Point & originalPoint) const
	{
		return originalPoint;
	}
}


