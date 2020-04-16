#ifndef CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER
#define CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/point.h>
#include <core/vector.h>
#include <rt/intersection.h>
#include <core/interpolate.h>

namespace rt {

class TriangleMapper : public CoordMapper {
public:
    TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2);
    TriangleMapper(Point ntv[3]);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Point tv0, tv1, tv2;
};

}



#endif