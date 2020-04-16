#include <rt/solids/disc.h>
#include <rt/solids/infiniteplane.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->center = center;
    this->normal = normal.normalize();
    this->radius = radius;
}

BBox Disc::getBounds() const {
    Vector spanX, spanY;
    
    Vector X_reference_vector = Vector(1, 0, 0);
    Vector Y_reference_vector = Vector(0, 1, 0);

    spanX = cross(X_reference_vector, this->normal);
    spanY = cross(spanX, this->normal);

    Point corner1 = Point(this->center + this->radius*spanX);
    Point corner2 = Point(this->center - this->radius*spanX);

    BBox bbox = BBox(min(corner1, corner2), max(corner1, corner2));

    Point corner3 = Point(this->center + this->radius*spanY);
    Point corner4 = Point(this->center - this->radius*spanY);

    bbox.extend(corner3);
    bbox.extend(corner4);

    return bbox;
    
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    InfinitePlane plane = InfinitePlane(this->center, this->normal, nullptr, material);
    Intersection intersect = plane.intersect(ray, previousBestDistance);
    if(intersect.if_intersect)
    {
        if( (this->center - intersect.hitPoint()).lensqr() > (this->radius*this->radius) )
        {
            intersect.if_intersect = false;
        }
    }
    intersect.local() = ray.getPoint(intersect.distance);
    return intersect;
}

Solid::Sample Disc::sample() const {
    NOT_IMPLEMENTED;
}

float Disc::getArea() const {
    return pi * this->radius * this->radius;
}

}