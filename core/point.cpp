#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z){
    
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point(const Float4& f4){
    this->x = f4.x/f4.w;
    this->y = f4.y/f4.w;
    this->z = f4.z/f4.w;
}

Vector Point::operator - (const Point& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

bool Point::operator == (const Point& b) const {
    return this->x==b.x && this->y==b.y && this->z==b.z;
}

bool Point::operator != (const Point& b) const {
    return !this->operator==(b);
}


float Point::getValue(int axis) const
{
  //  std::cout<<axis;
   switch (axis)
   {
   case x_axis:
       return this->x;
       break;
   case y_axis:
       return this->y;
       break;
   case z_axis:
       return this->z;
       break;
   default:
       std::cout<<"error in get value, only have x,y,z!"<<std::endl;
       assert(false);
       return -1.0; 
       break;
   }
}

Point operator * (float scalar, const Point& b) {
    return Point(scalar*b.x, scalar*b.y, scalar*b.z);
}

Point operator * (const Point& a, float scalar) {
    return Point(scalar*a.x, scalar*a.y, scalar*a.z);
}

Point operator+(const Point& a, const Point& b)
{
    return Point(a.x+b.x, a.y+b.y, a.z+b.z);
}

Point min(const Point& a, const Point& b) {
    return Point(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}

Point max(const Point& a, const Point& b) {
    return Point(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}

}
