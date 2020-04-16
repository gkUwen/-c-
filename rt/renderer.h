#ifndef CG1RAYTRACER_RENDERER_HEADER
#define CG1RAYTRACER_RENDERER_HEADER

#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <iostream>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/color.h>
#include <core/scalar.h>
#include <core/julia.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <core/random.h>

namespace rt {

class Image;
class Camera;
class Integrator;

class Renderer {
public:
    Renderer(Camera* cam, Integrator* integrator);
    void setSamples(uint samples) { this->samples = samples; }
    void render(Image& img);
    void test_render1(Image& img);
    void test_render2(Image& img);
    void test_render_bonus(Image& img);
private:
    Camera* cam;
    Integrator* integrator;
    uint samples;
};

}

#endif