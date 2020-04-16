#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    Point center;
    Vector forward_normalised;
    Vector up_normalised;
    Vector image_x_axis;// cross product of up and fwd
    Vector image_y_axis;// flip upward
    float vertical_OpeningAngle;
    float horizonal_OpeningAngle;
};

}


#endif