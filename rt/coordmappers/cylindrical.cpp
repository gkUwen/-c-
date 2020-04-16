#include <rt/coordmappers/cylindrical.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
    this->origin = origin;
    this->longitudinalAxis = longitudinalAxis;
    Vector tmp = polarAxis;
    tmp.z *= -1.0f;
    this->polarAxis = polarAxis;

    this->normal = cross(this->polarAxis, this->longitudinalAxis).normalize();
    this->transform = Matrix::system(1.0f*polarAxis.normalize(), 1.0f*longitudinalAxis.normalize(), 1.0*normal).invert();

    Point move = Point(-origin.x, -origin.y, -origin.z);
    // Matrix rotateX = Matrix(Float4(1, 0, 0, 0), 
    //                         Float4(0, cos(180*pi/180), -sin(180*pi/180), 0),
    //                         Float4(0, sin(180*pi/180), cos(180*pi/180), 0),
    //                         Float4(0, 0, 0, 1));
    // Matrix rotateY = Matrix(Float4(cos(180*pi/180), 0, sin(180*pi/180), 0), 
    //                         Float4(0, 1, 0, 0),
    //                         Float4(-sin(180*pi/180), 0, cos(180*pi/180), 0),
    //                         Float4(0, 0, 0, 1));
    // Matrix rotateZ = Matrix(Float4(cos(180*pi/180), -sin(180*pi/180), 0, 0), 
    //                         Float4(sin(180*pi/180), cos(180*pi/180), 0, 0),
    //                         Float4(0, 0, 1, 0),
    //                         Float4(0, 0, 0, 1));
    this->transform = product(this->transform.transpose(), translation(move));
    // this->transform = product(this->transform, rotateX);
    // this->transform = product(this->transform, rotateY);
    // this->transform = product(this->transform, rotateZ);
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    Point trans = this->transform * hit.hitPoint();
    float phi = 1.0f*atan2(trans.y, trans.x);

    Point tmp;
    tmp = Point(phi/(-2 * pi * this->polarAxis.length()), 1.0f*trans.z/this->longitudinalAxis.length(), 0);
    if (phi > 0){ 
        tmp = Point(phi/(-2 * pi * this->polarAxis.length()), 1.0f*trans.z/this->longitudinalAxis.length(), 0);
    }else{ 
        tmp = Point(phi/(2 * pi * this->polarAxis.length()), -1.0f*trans.z/this->longitudinalAxis.length(), 0);
    }
    
    return tmp;
}

}