
#include <rt/solids/infiniteplane.h>

#include <rt/solids/quad.h>
#include <rt/solids/disc.h>
#include <rt/solids/aabox.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/castingdist.h>
#include <rt/renderer.h>
#include <limits>

#define F_MAX std::numeric_limits<float>::max()
#define F_MIN -std::numeric_limits<float>::max()

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->origin = origin;
    this->normal = normal;
   // this->mat = material;
}

BBox InfinitePlane::getBounds() const {
    BBox bbox;

    return bbox.full();
}
Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    //t = (a-o)*n/(d*n)
    
    Vector l_oa = this->origin - ray.o;

    float num = dot(l_oa, this->normal);
    float den = dot(this->normal, ray.d);

    float t = num/den;
    Intersection intersect;
    intersect.if_intersect = false;


    if(t > 0 && t < previousBestDistance && fabs(den) > 0.001f)
    {
        if (dot(ray.d, this->normal) > 0){
            //return Intersection(t, ray, this, this->normal, this->getLocalPoint(ray.getPoint(t)));
            intersect = Intersection(t, ray, this, 1.0f*this->normal,ray.getPoint(t));
            Point tmp;
            tmp.x = intersect.hitPoint().x; - this->origin.x;
            tmp.y = intersect.hitPoint().y; - this->origin.y;
            tmp.z = intersect.hitPoint().z; - this->origin.z;
            //intersect.local() = tmp;
            intersect.localPoint = tmp;
        }else{
            intersect = Intersection(t, ray, this, 1.0f*this->normal, ray.getPoint(t));
            Point tmp;
            tmp.x = intersect.hitPoint().x; - this->origin.x;
            tmp.y = intersect.hitPoint().y; - this->origin.y;
            tmp.z = intersect.hitPoint().z; - this->origin.z;
            //intersect.local() = tmp;
            intersect.localPoint = tmp;
        }
    } 
    else
    {
        //Intersection intersect;
        intersect.if_intersect = false; 
        //return intersect;
    }
    
    return intersect;
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return F_MAX;
}
}
