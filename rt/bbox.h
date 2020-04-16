#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <float.h>
#include <rt/ray.h>
#include <math.h>

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;

    BBox() {}
    BBox(const Point& min, const Point& max)
    {
        this->min = min;
        this->max = max;
        this->if_empty = false;
        this->if_full  = false;
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return max - min;
    }

    std::pair<float, float> intersect(const Ray& ray) const;

    float getArea();

    bool isUnbound();
    bool isEmpty();

private:
    bool if_empty;
    bool if_full;
};

}

#endif