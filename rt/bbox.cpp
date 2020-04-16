#include <limits>
#include <iostream>
#include <cstddef>
#include <rt/bbox.h>
#include <algorithm> 

#define F_MAX std::numeric_limits<float>::max()
#define F_MIN -std::numeric_limits<float>::max()

namespace rt {

BBox BBox::empty() {
    BBox bbox;
    bbox.min = Point(F_MAX,F_MAX,F_MAX);
    bbox.max = Point(F_MIN, F_MIN, F_MIN);
    bbox.if_empty = true;
    return bbox;
}

BBox BBox::full() {
    BBox bbox;
    bbox.min = Point(F_MIN, F_MIN, F_MIN);
    bbox.max = Point(F_MAX, F_MAX, F_MAX);
    bbox.if_full = true;

    return bbox;
}


void BBox::extend(const Point& point) {
    
    this->min = rt::min(this->min, point);
    this->max = rt::max(this->max, point);
   
}

void BBox::extend(const BBox& bbox) {
    this->min = rt::min(this->min, bbox.min);
    this->max = rt::max(this->max, bbox.max);
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {

    if (this->min.x > this->max.x && this->min.y > this->max.y && this->min.z > this->max.z){
        return std::pair<float, float>(F_MIN, F_MIN);
    }
    else{
        Point t_near;
        Point t_far;

        t_near.x = (this->min.x - ray.o.x)/ray.d.x;
		
        t_near.y = (this->min.y - ray.o.y)/ray.d.y;
		
        t_near.z = (this->min.z - ray.o.z)/ray.d.z;

        t_far.x = (this->max.x - ray.o.x)/ray.d.x;
        t_far.y = (this->max.y - ray.o.y)/ray.d.y;
        t_far.z = (this->max.z - ray.o.z)/ray.d.z;

        if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
        if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);
        if (t_near.z > t_far.z) std::swap(t_near.z, t_far.z);

        float max_t_near = std::max(t_near.z, std::max(t_near.x, t_near.y));
        float min_t_far  = std::min(std::min(t_far.x, t_far.y), t_far.z); 

        return std::pair<float, float>(max_t_near, min_t_far);
    }
}

bool BBox::isUnbound() {
    
    bool flag1 = this->min.x == F_MIN || this->max.x == F_MAX;
	bool flag2 = this->min.y == F_MIN || this->max.y == F_MAX;
	bool flag3 = this->min.z == F_MIN || this->max.z == F_MAX;

	return (flag1 || flag2 || flag3);
}

bool BBox::isEmpty() 
{
    
     return this->min.x == FLT_MAX || this->min.y == FLT_MAX || this->min.z == FLT_MAX
         || this->max.x == FLT_MIN || this->max.y == FLT_MIN || this->max.z == FLT_MIN;

}

float BBox::getArea()
{
    if(this->isEmpty())
    {
        return 0.0;
    }
    
    float l1 = fabsf(this->max.x - this->min.x);
	float l2 = fabsf(this->max.y - this->min.y);
	float l3 = fabsf(this->max.z - this->min.z);

	return 2 * (l1*l2 + l1*l3 + l2*l3);
}

}
