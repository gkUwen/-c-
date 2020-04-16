#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->norm = normal.normalize();
    this->localPoint = local;
    this->if_intersect = true;
}

Intersection::operator bool() {
    return this->if_intersect;
}

Intersection Intersection::failure() {
    Intersection intersection;
    intersection.if_intersect = false;
    return intersection;
}

Point Intersection::hitPoint() const {
    return ray.getPoint(this->distance);
}

Vector Intersection::normal() const {
    return this->norm;
}

Point Intersection::local() const{
    return this->localPoint;
}

}
