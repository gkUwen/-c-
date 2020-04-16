#include <rt/coordmappers/tmapper.h>

namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
    this->tv0 = ntv[0];
    this->tv1 = ntv[1];
    this->tv2 = ntv[2];
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
    this->tv0 = tv0;
    this->tv1 = tv1;
    this->tv2 = tv2;
}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    
  return lerpbar(this->tv0, this->tv1, this->tv2, hit.local().x, hit.local().y);
}

}
