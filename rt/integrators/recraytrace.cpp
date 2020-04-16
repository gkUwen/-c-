#include <rt/integrators/recraytrace.h>
#include <rt/materials/material.h>
#include <rt/world.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/ray.h>
#include <core/color.h>


namespace rt {

RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray, int depth) const {
	if (depth > MAXDEPTH) {
		return RGBColor(0, 0, 0);
	}

	Intersection intersection = world->scene->intersect(ray);
	RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
	float OFFSET = 1e-6f;

	if (intersection.if_intersect) {

		Vector normal = intersection.normal().normalize();
		Point texturePoint = intersection.solid->texMapper->getCoords(intersection);

		if (dot(normal, ray.d) > 0.001f) normal = -normal;

		Material::Sampling samplingMode = intersection.solid->material->useSampling();
		Material::SampleReflectance sampleReflect;

		Ray shadowRay;

		if (samplingMode == Material::SAMPLING_NOT_NEEDED) {
			for (int i = 0; i < this->world->light.size(); i++){
            	LightHit light_hit = this->world->light[i]->getLightHit(intersection.hitPoint());

				if (dot(normal, light_hit.direction.normalize()) < 0.001f) continue;

				shadowRay.o = intersection.hitPoint() + OFFSET * normal;
				shadowRay.d = light_hit.direction;
				Intersection shadow_intersect = world->scene->intersect(shadowRay, light_hit.distance);
				if (!shadow_intersect.if_intersect) {
					RGBColor reflect = intersection.solid->material->getReflectance(texturePoint, normal, -ray.d, shadowRay.d);
					color = color + reflect * this->world->light[i]->getIntensity(light_hit);
				}
			}
		}
		else if (samplingMode == Material::SAMPLING_ALL) {
			sampleReflect = intersection.solid->material->getSampleReflectance(texturePoint, normal, -ray.d);
			if (sampleReflect.reflectance.r > 0.001f) {
				Ray secondary;
				secondary.o = intersection.hitPoint() + OFFSET * normal;
				secondary.d = sampleReflect.direction;
				color = color + sampleReflect.reflectance * getRadiance(secondary, depth + 1);
			}
		}
		else if (samplingMode == Material::SAMPLING_SECONDARY) {
			for (int i = 0; i < this->world->light.size(); i++){
            	LightHit light_hit = this->world->light[i]->getLightHit(intersection.hitPoint());

				if (dot(normal, light_hit.direction.normalize()) < 0.001f) continue;

				shadowRay.o = intersection.hitPoint() + OFFSET * normal;
				shadowRay.d = light_hit.direction;
				Intersection shadow_intersect = world->scene->intersect(shadowRay, light_hit.distance);
				if (shadow_intersect.if_intersect) {
					RGBColor reflect = intersection.solid->material->getReflectance(texturePoint, normal, -ray.d, shadowRay.d);
					color = color + reflect * this->world->light[i]->getIntensity(light_hit);
				}
			}
			sampleReflect = intersection.solid->material->getSampleReflectance(texturePoint, normal, -ray.d);
			if (sampleReflect.reflectance.r > 0.001f) {
				Ray secondary;
				secondary.o = intersection.hitPoint() + OFFSET * sampleReflect.direction;
				secondary.d = sampleReflect.direction;
				color = color + sampleReflect.reflectance * getRadiance(secondary, depth + 1);
			}
		}

		RGBColor emission = intersection.solid->material->getEmission(texturePoint, intersection.normal(), -ray.d);

		color = color + emission;

		return color;
	}
	return RGBColor(0, 0, 0);
}

}

// #include "recraytrace.h"
// #include <rt/coordmappers/world.h>
// #include <core/color.h>
// #include <core/vector.h>
// #include <rt/lights/light.h>
// #include <rt/intersection.h>
// #include <rt/solids/solid.h>
// #include <rt/materials/material.h>
// #include <rt/materials/combine.h>
// #include <rt/coordmappers/coordmapper.h>

// namespace rt
// {
// 	RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray & ray, int depth) const
// 	{
// 		float displacement = 1e-5f;
// 		RGBColor color = RGBColor(0, 0, 0);
// 		if (depth >= MAXDEPTH)
// 			return color;

// 		Intersection intersection = this->world->scene->intersect(ray);

// 		if (intersection)
// 		{
// 			Vector normal = intersection.normal().normalize();

// 			auto texMap = intersection.solid->texMapper;
// 			if (texMap == nullptr)
// 				texMap = new WorldMapper();

// 			Point texturePoint = texMap->getCoords(intersection);

// 			auto currentMaterial = intersection.solid->material;
// 			auto outDirFlipped = ray.d.normalize();
// 			// This vector should ideally be directed towards the other way but 
// 			// due to risk of regression it might stay flipped forever :)

// 			if (currentMaterial->useSampling() == Material::Sampling::SAMPLING_NOT_NEEDED)
// 			{
// 				for (auto lightSource : this->world->light)
// 				{
// 					LightHit lightHit = lightSource->getLightHit(intersection.hitPoint());
// 					Vector inDir = lightHit.direction;
// 					if (dot(inDir, normal) < 0)
// 						continue;

// 					Ray shadowRay = Ray(intersection.hitPoint() + displacement * inDir, inDir);
// 					Intersection shadowRayIntersection = this->world->scene->intersect(shadowRay, lightHit.distance - 1.1 * displacement);
// 					if (shadowRayIntersection)
// 						continue;

// 					RGBColor reflected = currentMaterial->getReflectance(texturePoint, normal, outDirFlipped, inDir);
// 					RGBColor lightIntensity = lightSource->getIntensity(lightHit);
// 					color = color + reflected * lightIntensity;
// 				}
// 			}

// 			else if (currentMaterial->useSampling() == Material::Sampling::SAMPLING_ALL)
// 			{
// 				auto reflectance = currentMaterial->getSampleReflectance(texturePoint, normal, outDirFlipped);
// 				Vector inDir = reflectance.direction.normalize();

// 				if (reflectance.reflectance.grayscale() > 0)
// 				{
// 					float dotP = dot(normal, inDir);
// 					int signOfEpsilon = 1;
// 					if (dotP < 0)
// 					{
// 						signOfEpsilon = -1;
// 					}

// 					Point rayOrigin = intersection.hitPoint();

// 					rayOrigin = currentMaterial->getRayOrigin(rayOrigin);

// 					Ray shadowRay = Ray(rayOrigin + signOfEpsilon * displacement * normal, inDir);

// 					RGBColor incomingLightColor = this->getRadiance(shadowRay, depth + 1);
// 					color = incomingLightColor * reflectance.reflectance;
// 				}
// 			}

// 			else if (currentMaterial->useSampling() == Material::Sampling::SAMPLING_SECONDARY)
// 			{
// 				for (auto lightSource : this->world->light)
// 				{
// 					LightHit lightHit = lightSource->getLightHit(intersection.hitPoint());
// 					Vector inDir = lightHit.direction;
// 					if (dot(inDir, normal) < 0)
// 						continue;

// 					Ray shadowRay = Ray(intersection.hitPoint() + displacement * inDir, inDir);
// 					Intersection shadowRayIntersection = this->world->scene->intersect(shadowRay, lightHit.distance);
// 					if (shadowRayIntersection)
// 						continue;

// 					RGBColor reflected = currentMaterial->getReflectance(texturePoint, normal, outDirFlipped, inDir);
// 					color = color + reflected * lightSource->getIntensity(lightHit);
// 				}

// 				auto reflectance = currentMaterial->getSampleReflectance(texturePoint, normal, outDirFlipped);
// 				Vector inDir = reflectance.direction.normalize();

// 				Point rayOrigin = intersection.hitPoint();
// 				Ray shadowRay = Ray(rayOrigin + displacement * inDir, inDir);

// 				RGBColor incomingLightColor = this->getRadiance(shadowRay, depth + 1);
// 				color = color + incomingLightColor * reflectance.reflectance;
// 			}
// 			color = color + intersection.solid->material->getEmission(texturePoint, normal, ray.d.normalize());
// 		}

// 		return color;
// 	}
// }