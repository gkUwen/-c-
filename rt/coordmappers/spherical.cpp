#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{
    this->origin = origin;
    this->zenith = zenith;
    this->azimuthRef = azimuthRef;

    this->normal = cross(this->azimuthRef, this->zenith).normalize();
    this->transform = Matrix::system(azimuthRef.normalize(), zenith.normalize(), this->normal).invert();

    Point move = Point(-origin.x, -origin.y, -origin.z);
    this->transform = product(this->transform.transpose(), translation(move));
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Point trans = this->transform * hit.local();
    float phi = 1.0f*atan(trans.y/trans.x);
    
    float r = sqrt(trans.x*trans.x + trans.y*trans.y + trans.z*trans.z);
    float theta = 1.0f*acos(trans.z / r);
    
    return Point (phi/(2*pi*this->zenith.length()), -1.0f*theta/(pi*this->azimuthRef.length()), 0);
}

}