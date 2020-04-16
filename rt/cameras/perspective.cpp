#include <rt/cameras/perspective.h>
#include <math.h>
#include <cmath>
#include <core/image.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/scalar.h>
#include <core/julia.h>
#include <rt/ray.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/orthographic.h>
#include <iostream>
#include <rt/renderer.h>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle)
{
    // Pre-process vectors
    this->vertical_OpeningAngle = verticalOpeningAngle;
    this->horizonal_OpeningAngle = horizontalOpeningAngle;
    Vector _forward = forward.normalize();
    Vector _up = up.normalize();
    Vector _image_x_axis = cross(forward, up).normalize();
    Vector _image_y_axis = cross(_image_x_axis, _forward).normalize(); // cross prod between _right x _forward
    
    // Compute Span[X/Y]
    float spanY = tan(this->vertical_OpeningAngle/2.0f);
    float spanX = tan(this->horizonal_OpeningAngle/2.0f);

    // Set framework vectors
    this->center = center;
    this->forward_normalised = _forward;
    this->up_normalised = _image_y_axis;
    this->image_y_axis = _image_y_axis*spanY;
    this->image_x_axis = _image_x_axis*spanX;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {   
    return Ray(this->center, (this->forward_normalised + this->image_x_axis*x + this->image_y_axis*y).normalize());
}

}
