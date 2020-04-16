#include <rt/textures/checkerboard.h>
#include <math.h>


namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
{
    this->black = black;
    this->white = white;
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {

    int lx = floor(2 * coord.x + 30000);
    int ly = floor(2 * coord.y + 30000);
    int lz = floor(2 * coord.z + 30000);

    int parity = abs(lx + ly + lz) % 2;
    if(parity == 0)
        return this->white;    
    else
        return this->black; 
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

}