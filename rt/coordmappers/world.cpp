#include <rt/coordmappers/world.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    Point hit_point = hit.hitPoint();
    
    return Point(hit_point.x*scale.x, hit_point.y*scale.y, hit_point.z*scale.z);
}

WorldMapper::WorldMapper()
{
    this->scale = Float4(1.0f,1.0f,1.0f,1.0f);
}

WorldMapper::WorldMapper(const Float4& scale)
{
    this->scale = scale;
}

}