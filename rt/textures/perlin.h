#ifndef CG1RAYTRACER_TEXTURES_PERLIN_HEADER
#define CG1RAYTRACER_TEXTURES_PERLIN_HEADER

#include <rt/textures/texture.h>
#include <core/color.h>
#include <core/scalar.h>
#include <vector>
#include <core/point.h>
#include <core/interpolate.h>
#include <core/scalar.h>
#include <math.h>
#include <core/interpolate.h>
#include <core/vector.h>
using namespace std;

namespace rt {

class PerlinTexture : public Texture {
public:
    PerlinTexture(const RGBColor& white, const RGBColor& black);
    void addOctave(float amplitude, float frequency);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
private:
    RGBColor black, white;
    vector<pair<float, float>> octaves;
	float getInterpolatedNoise(const Point& coord, float freq);
};

}

#endif