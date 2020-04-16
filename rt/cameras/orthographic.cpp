#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
    // Pre-process vectors.
    Vector _forward = forward.normalize();
    Vector _up = up.normalize();
    Vector _image_x_axis = cross(_forward, _up).normalize();
    Vector _image_y_axis = _up;

    // Set framework vectors.
    this->center = center;
    this->forward_normmalised = _forward;
    this->up_normalised = cross(_forward, _image_x_axis).normalize();
    this->image_y_axis = _image_y_axis*(scaleY*0.5f);
    this->image_x_axis = _image_x_axis*(scaleX*0.5f);
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    return Ray(this->center + x*this->image_x_axis + y*this->image_y_axis, this->forward_normmalised);
}

}
