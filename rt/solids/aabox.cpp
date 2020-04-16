#include <rt/solids/aabox.h>

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->corner1 = corner1;
    this->corner2 = corner2;

}

BBox AABox::getBounds() const {
    return BBox(min(corner1, corner2), max(corner1, corner2));
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    float l1 = abs(corner1.x - corner2.x);
    float l2 = abs(corner1.y - corner2.y);
    float l3 = abs(corner1.z - corner1.z);

    return 2*(l1*l2 + l2*l3 + l3*l1);
}

Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {

    using namespace std;//

    Point t_near;
    Point t_far;

    t_near.x = (this->corner1.x - ray.o.x)/ray.d.x;
    t_near.y = (this->corner1.y - ray.o.y)/ray.d.y;
    t_near.z = (this->corner1.z - ray.o.z)/ray.d.z;

    t_far.x = (this->corner2.x - ray.o.x)/ray.d.x;
    t_far.y = (this->corner2.y - ray.o.y)/ray.d.y;
    t_far.z = (this->corner2.z - ray.o.z)/ray.d.z;

    if (t_near.x > t_far.x) swap(t_near.x, t_far.x);
    if (t_near.y > t_far.y) swap(t_near.y, t_far.y);
    if (t_near.z > t_far.z) swap(t_near.z, t_far.z);

    float max_t_near = max(t_near.z, max(t_near.x, t_near.y));
    float min_t_far =  min(min(t_far.x, t_far.y), t_far.z); 

    Intersection intersect;
    intersect.if_intersect = false;
    if (max_t_near < min_t_far)
    {
        float t = max_t_near;

        if (t > 0 && t < previousBestDistance){
            Vector normalised;
            if (t == t_near.x)
            {
               normalised = Vector(-ray.d.x, 0, 0).normalize();
            }
            else if(t == t_near.y) 
            {
               normalised = Vector(0, -ray.d.y, 0).normalize();
            }
            else
            {
               normalised = Vector(0, 0, -ray.d.z).normalize();
            }
            
            intersect = Intersection(t, ray, this, normalised, ray.getPoint(t));
            intersect.local() = ray.getPoint(t);
        }
    }

    return intersect;
  
}
}
