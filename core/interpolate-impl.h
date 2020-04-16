#include "interpolate.h"

namespace rt {

template <typename T>
T lerp(const T& px0, const T& px1, float xPoint) {
    //return px0*xPoint + px1*(1-xPoint);
    return px0*(1-xPoint) + px1*xPoint;
}

template <typename T>
T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
   return a*aWeight + b*bWeight + c*(1-aWeight-bWeight);
}

template <typename T>
T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
    T y0 = lerp(px0y0, px1y0, xWeight);
    T y1 = lerp(px0y1, px1y1, xWeight);

    return lerp(y0, y1, yWeight);
}

template <typename T>
T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
    const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
    float xPoint, float yPoint, float zPoint)
{
    T z0_plane = lerp2d(px0y0z0,px1y0z0,px0y1z0,px1y1z0,xPoint,yPoint);
    T z1_plane = lerp2d(px0y0z1,px1y0z1,px0y1z1,px1y1z1,xPoint,yPoint);

    return lerp(z0_plane,z1_plane,zPoint);
}

}