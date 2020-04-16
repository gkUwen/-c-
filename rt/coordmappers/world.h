#ifndef CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER
#define CG1RAYTRACER_COORDMAPPERS_WORLD_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/vector.h>
#include <core/float4.h>
#include <rt/intersection.h>


namespace rt {

class Float4;

class WorldMapper : public CoordMapper {
public:
    WorldMapper();
    explicit WorldMapper(const Float4& scale);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Float4 scale;
};

}

#endif