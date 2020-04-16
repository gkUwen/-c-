#ifndef CG1RAYTRACER_POINT_HEADER
#define CG1RAYTRACER_POINT_HEADER

#include <core/assert.h>
#include <core/macros.h>
#include <assert.h>

#define x_axis 0
#define y_axis 1
#define z_axis 2

namespace rt {

class Vector;
class Float4;

class ALIGN(16) Point {
public:
    float x, y, z;

    Point() = default;
    Point(float x, float y, float z);
    explicit Point(const Float4& f4);

    static Point rep(float v) { return Point(v,v,v); }

    Vector operator-(const Point& b) const;

    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;
    float getValue(int axis) const;
};

Point operator*(float scalar, const Point& b);
Point operator*(const Point& a, float scalar);
Point operator+(const Point& a, const Point& b);

Point min(const Point& a, const Point& b);
Point max(const Point& a, const Point& b);

}

#endif