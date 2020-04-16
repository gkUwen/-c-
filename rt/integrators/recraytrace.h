#ifndef CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER

#include <rt/integrators/integrator.h>
#include <rt/intersection.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/world.h>
#include <rt/materials/material.h>

namespace rt {

class RecursiveRayTracingIntegrator : public Integrator {
public:
    RecursiveRayTracingIntegrator(World* world) : Integrator(world) {}
   	virtual RGBColor getRadiance(const Ray& ray) const { return getRadiance(ray, 0); };
	
	RGBColor getRadiance(const Ray& ray, int depth=0) const;

   	int MAXDEPTH = 6;
};

 }

#endif
