#ifndef CG1RAYTRACER_INTEGRATORS_VOLUME_HEADER
#define CG1RAYTRACER_INTEGRATORS_VOLUME_HEADER

#include <rt/integrators/integrator.h>

namespace rt {
class World;
class Ray;
class RGBColor;

class volume : public Integrator {
public:
    volume(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const;
};

}

#endif