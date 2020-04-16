#include <rt/solids/sphere.h>
#include <iostream>
#include <core/point.h>
#include <rt/bbox.h>
#include <core/assert.h>
#include <rt/intersection.h>
#include <cmath>
#include <limits>
#include <iostream>
#include <cstddef>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->center = center;
    this->radius = radius;
   // this->mat = material;
}

BBox Sphere::getBounds() const {    

    return BBox(Point(this->center.x - this->radius, this->center.y - this->radius, this->center.z - this->radius),
                Point(this->center.x + this->radius, this->center.y + this->radius, this->center.z + this->radius));
    
}

Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {

    float a = dot(ray.d.normalize(), ray.d.normalize()); 
    float b = 2.0f*dot(ray.d.normalize(), ray.o-this->center);
    float c = dot(ray.o-this->center, ray.o-this->center) - (this->radius * this->radius);

    float delta = b*b - 4*a*c;
    Intersection intersect;
    intersect.if_intersect = false;

    if(delta > 0.0001f)
    {
        float t1 = (-b - sqrtf(delta))/ (2*a);
        float t2 = (-b + sqrtf(delta))/ (2*a);
        Vector normal_vector;
        
        if(t1 <= t2 && t1 >= 0.001f && t1 <= previousBestDistance)
        {
            normal_vector = (ray.getPoint(t1) - this->center).normalize();
            intersect = Intersection(t1, ray, this, normal_vector, ray.getPoint(t1));
            intersect.local() = ray.getPoint(t1);
        }
        else if(t2 < t1 && t2 >= 0.001f && t2 <= previousBestDistance)
        {
            normal_vector = (ray.getPoint(t2) - this->center).normalize();
            intersect = Intersection(t2, ray, this, normal_vector, ray.getPoint(t2));
            intersect.local() = ray.getPoint(t2);
        }
       
    }
    // else

    // if(fabs(delta) >= 0.001f)
    // {
    //     float t1 = -b/ (2*a);
    //     Vector normal_vector;

    //     normal_vector = (ray.getPoint(t1) - this->center).normalize();
    //     intersect = Intersection(t1, ray, this, normal_vector, ray.getPoint(t1));

    //     return intersect;

    //     // float t1 = (-b - sqrtf(delta))/ (2*a);
    //     // float t2 = (-b + sqrtf(delta))/ (2*a);
    //     // Vector normal_vector;
        
    //     // if(t1 <= t2 && t1 >= 0 && t1 < previousBestDistance)
    //     // {
    //     //     normal_vector = (ray.getPoint(t1) - this->center).normalize();
    //     //     intersect = Intersection(t1, ray, this, normal_vector, ray.getPoint(t1));
    //     // }
    //     // else if(t2 < t1 && t2 >= 0 && t2 < previousBestDistance)
    //     // {
    //     //     normal_vector = (ray.getPoint(t2) - this->center).normalize();
    //     //     intersect = Intersection(t2, ray, this, normal_vector, ray.getPoint(t2));
    //     // }
    // }

    return intersect;
}   

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    return 4*pi * this->radius * this->radius;
}
}

