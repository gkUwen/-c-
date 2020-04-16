#include <rt/coordmappers/plane.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    this->e1 = e1;
    this->e2 = e2;
    Vector normal = cross(e1,e2).normalize();
    this->transform = Matrix::system(e1.normalize(),e2.normalize(),normal).invert();
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    Point trans = this->transform * hit.local(); 
    
    return Point(trans.x/this->e1.length(), trans.y/this->e2.length(),0);
}

}