#include <rt/textures/perlin.h>
#include <core/point.h>
#include <stdlib.h>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
{
    this->white = white;
    this->black = black;
    this->octaves.clear();
}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
   
    float f, a, turb = 0.0f;

    for (int i = 0; i < octaves.size(); i++)
    {
        a = octaves[i].first;
        f = octaves[i].second;
        turb = turb + a*getInterpolatedNoise(coord, f);
    }

    return lerp(this->black, this->white, turb);
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    std::pair<float, float> octave(amplitude, frequency);
	this->octaves.push_back(octave);
}

float PerlinTexture::getInterpolatedNoise(const Point& coord, float freq)
{
    Point c = coord*freq;
    float lu = floor(c.x), lv = floor(c.y), lw = floor(c.z);
    float fu = (c.x - lu), fv = (c.y - lv), fw = (c.z - lw);

    float u0v0w0 = (noise(lu, lv, lw) + 1.0f)/2.0f;
    float u0v0w1 = (noise(lu, lv, lw+1) + 1.0f)/2.0f;
    float u0v1w0 = (noise(lu, lv+1, lw) + 1.0f)/2.0f;
    float u0v1w1 = (noise(lu, lv+1, lw+1) + 1.0f)/2.0f;
    float u1v0w0 = (noise(lu+1, lv, lw) + 1.0f)/2.0f;
    float u1v0w1 = (noise(lu+1, lv, lw+1) + 1.0f)/2.0f;
    float u1v1w0 = (noise(lu+1, lv+1, lw) + 1.0f)/2.0f;
    float u1v1w1 = (noise(lu+1, lv+1, lw+1) + 1.0f)/2.0f;

    // float noise_array[8];

    // float u0v0w0 = noise(lu, lv, lw);
    // float u0v0w1 = noise(lu, lv, lw+1);
    // float u0v1w0 = noise(lu, lv+1, lw);
    // float u0v1w1 = noise(lu, lv+1, lw+1); 
    // float u1v0w0 = noise(lu+1, lv, lw);
    // float u1v0w1 = noise(lu+1, lv, lw+1);
    // float u1v1w0 = noise(lu+1, lv+1, lw);
    // float u1v1w1 = noise(lu+1, lv+1, lw+1); 

    // noise_array[0] = u0v0w0;
    // noise_array[1] = u0v0w1;
    // noise_array[2] = u0v1w0;
    // noise_array[3] = u0v1w1;
    // noise_array[4] = u1v0w0;
    // noise_array[5] = u1v0w1;
    // noise_array[6] = u1v1w0;
    // noise_array[7] = u1v1w1;

    // float maxval = max(fabs(u1v1w0), fabs(u1v1w1));
    // maxval = max(maxval, fabs(u1v0w1));
    // maxval = max(maxval, fabs(u1v0w0));
    // maxval = max(maxval, fabs(u0v1w1));
    // maxval = max(maxval, fabs(u0v1w0));
    // maxval = max(maxval, fabs(u0v0w1));
    // maxval = max(maxval, fabs(u0v0w0));

    // float minval = min(fabs(u1v1w0), fabs(u1v1w1));
    // minval = min(minval, fabs(u1v0w1));
    // minval = min(minval, fabs(u1v0w0));
    // minval = min(minval, fabs(u0v1w1));
    // minval = min(minval, fabs(u0v1w0));
    // minval = min(minval, fabs(u0v0w1));
    // minval = min(minval, fabs(u0v0w0));
    
    // std::cout << minval << "/" << maxval << std::endl;

    // for (int i = 0; i < 8; i++){
    //     if (noise_array[i] < 0) noise_array[i] = 0;
    //     if (noise_array[i] > 0){
    //         noise_array[i] = (noise_array[i] + maxval)/(maxval + 1.0f);
            
    //     }
    // }
    
    // std::cout << u0v0w0 << "--";
    // std::cout << u0v0w1 << "--";
    // std::cout << u0v1w0 << "--";
    // std::cout << u0v1w1 << "--";
    // std::cout << u1v0w0 << "--";
    // std::cout << u1v0w1 << "--";
    // std::cout << u1v1w0 << "--";
    // std::cout << u1v1w1;
    // std::cout << std::endl;    

    // float max_ = min(maxval, fabs(minval));
    // u0v0w0 = (u0v0w0 + max_)/(2.0f*max_);
    // u0v0w1 = (u0v0w1 + max_)/(2.0f*max_);
    // u0v1w0 = (u0v1w0 + max_)/(2.0f*max_);
    // u0v1w1 = (u0v1w1 + max_)/(2.0f*max_); 
    // u1v0w0 = (u1v0w0 + max_)/(2.0f*max_);
    // u1v0w1 = (u1v0w1 + max_)/(2.0f*max_);
    // u1v1w0 = (u1v1w0 + max_)/(2.0f*max_);
    // u1v1w1 = (u1v1w1 + max_)/(2.0f*max_);

    // std::cout << u0v0w0 << "/";
    // std::cout << u0v0w1 << "/";
    // std::cout << u0v1w0 << "/";
    // std::cout << u0v1w1 << "/";
    // std::cout << u1v0w0 << "/";
    // std::cout << u1v0w1 << "/";
    // std::cout << u1v1w0 << "/";
    // std::cout << u1v1w1;
    // std::cout << std::endl;

    float tmp = lerp3d(u0v0w0, u1v0w0, u0v1w0, u1v1w0, u0v0w1, u1v0w1, u0v1w1, u1v1w1, fu, fv, fw);
    // float tmp = lerp3d(noise_array[0], noise_array[1], noise_array[2], noise_array[3],
    //                    noise_array[4], noise_array[5], noise_array[6], noise_array[7], fu, fv, fw);

    return lerp3d(u0v0w0, u1v0w0, u0v1w0, u1v1w0, u0v0w1, u1v0w1, u0v1w1, u1v1w1, fu, fv, fw);

}

}