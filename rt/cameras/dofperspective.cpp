#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <math.h>
#include <core/scalar.h>
#include <core/random.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
{
    this->center =center;
    this->forward = forward.normalize();
    this->up= up.normalize();

    this->focalDistance = focalDistance;
    this->apertureRadius = apertureRadius;

    this->image_x_axis = cross(forward, up).normalize()*tanf(horizontalOpeningAngle / 2);
    this->image_y_axis = cross(this->image_x_axis, forward).normalize()*tanf(verticalOpeningAngle / 2);
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector dir = (this->forward + x*this->image_x_axis + y*this->image_y_axis).normalize();

    float angle = 2*pi*random();
    float radius = this->apertureRadius*random();

    float x_polor = cos(angle)*radius;
    float y_polor = sin(angle)*radius;

    Point variant_focalpoint = this->center + x_polor*image_x_axis + y_polor*image_y_axis;
    Point focalpoint = Ray(this->center, dir).getPoint(this->focalDistance);

    return Ray(variant_focalpoint, (focalpoint - variant_focalpoint).normalize());
    
}

}
