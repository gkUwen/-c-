#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>
#include <assert.h>

namespace rt {

Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const Float4& f4)
{
    assert(f4.w == 0);

    this->x = f4.x;
    this->y = f4.y;
    this->z = f4.z;
}

Vector Vector::operator + (const Vector& b) const {
    return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector Vector::operator - (const Vector& b) const {
    return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

Vector Vector::operator - () const {
    return Vector(-this->x, -this->y, -this->z);
}

Vector Vector::normalize() const {
    float len = this->length();
    return Vector(this->x/len, this->y/len, this->z/len);
}

Vector operator * (float scalar, const Vector& b) {
    return Vector(scalar*b.x, scalar*b.y, scalar*b.z);
}

Vector operator * (const Vector& a, float scalar) {
    return Vector(scalar*a.x, scalar*a.y, scalar*a.z);
}

Vector operator / (const Vector& a, float scalar) {
    return Vector(1/scalar*a.x, 1/scalar*a.y, 1/scalar*a.z);
}

Vector cross(const Vector& a, const Vector& b) {
    return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float dot(const Vector& a, const Vector& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Vector::lensqr() const {
    return  this->x*this->x + this->y*this->y + this->z*this->z;
}

float Vector::length() const {
    return sqrt(lensqr());
}


bool Vector::operator == (const Vector& b) const {
    return this->x==b.x && this->y==b.y && this->z==b.z;
}

bool Vector::operator != (const Vector& b) const {
    return !this->operator==(b);
}

Vector min(const Vector& a, const Vector& b) {
    return Vector(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}

Vector max(const Vector& a, const Vector& b) {
    return Vector(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}

Point operator + (const Point& a, const Vector& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator + (const Vector& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator - (const Point& a, const Vector& b) {
    return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator * (const Float4& scale, const Point& p) {
    NOT_IMPLEMENTED;//don't need in this assignment
}

}
