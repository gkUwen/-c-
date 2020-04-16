#include <rt/primmod/instance.h>
#include <core/image.h>
#include <rt/cameras/perspective.h>
#include <rt/groups/group.h>
#include <rt/groups/simplegroup.h>
#include <rt/primmod/instance.h>
#include <rt/integrators/casting.h>
#include <rt/world.h>
#include <rt/renderer.h>

#include <rt/solids/infiniteplane.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>


namespace rt {

Instance::Instance(Primitive* content)
{
    this->instance = content;   
    this->reset();
}

Primitive* Instance::content() {
    return this->instance;
}

void Instance::reset() {
    this->transform = Matrix::identity();
    this->inv_transform = Matrix::identity();
}

void Instance::translate(const Vector& t) {
    Matrix __translate_matrix = Matrix(Float4(1.0f, 0.0f, 0.0f, t.x),
                                       Float4(0.0f, 1.0f, 0.0f, t.y),
                                       Float4(0.0f, 0.0f, 1.0f, t.z),
                                       Float4(0.0f, 0.0f, 0.0f, 1.0f));

    this->transform = product(__translate_matrix, this->transform);
    this->inv_transform = this->transform.invert();
}

void Instance::rotate(const Vector& nnaxis, float angle) {

    Vector r = nnaxis.normalize();
    
    Matrix __rotation_matrix = Matrix::identity();

    Vector s, t;

    if (min(min(r.x, r.y), r.z) == r.x){
        s.x = 0.0f;
        s.y = -1.0f*r.z;
        s.z = r.y;
    }

    if (min(min(r.x, r.y), r.z) == r.y){
        s.x = -1.0f*r.z;
        s.y = 0.0f;
        s.z = r.x;
    }

    if (min(min(r.x, r.y), r.z) == r.z){
        s.x = -1.0f*r.y;
        s.y = r.x;
        s.z = 0.0f;
    }

    s = s.normalize();
    t = cross(r, s).normalize();

    Matrix R_x = Matrix(Float4(1.0f, 0.0f, 0.0f, 0.0f),
                        Float4(0.0f, cos(angle), -1.0f*sin(angle), 0),
                        Float4(0.0f, sin(angle), cos(angle), 0),
                        Float4(0.0f, 0.0f, 0.0f, 1.0f));

  
    Matrix M = Matrix::system(r, s, t);
    
    __rotation_matrix = product(M, product(R_x, M.transpose()));
    
    this->transform = product(__rotation_matrix, this->transform);
    this->inv_transform = this->transform.invert();

}


void Instance::scale(float f) {

    Matrix __scale_matrix = Matrix(Float4(f, 0.0f, 0.0f , 0.0f),
                                   Float4(0.0f, f, 0.0f , 0.0f),
                                   Float4(0.0f, 0.0f, f , 0.0f),
                                   Float4(0.0f, 0.0f , 0.0f, 1.0f));

                
    this->transform = product(__scale_matrix, this->transform);
    this->inv_transform = this->transform.invert();

}

void Instance::scale(const Vector& s) {

    Matrix __scale_matrix = Matrix(Float4(s.x, 0.0f, 0.0f , 0.0f),
                                   Float4(0.0f, s.y, 0.0f , 0.0f),
                                   Float4(0.0f, 0.0f, s.z , 0.0f),
                                   Float4(0.0f, 0.0f , 0.0f, 1.0f));

    this->transform = product(__scale_matrix, this->transform);
    this->inv_transform = this->transform.invert();
}

void Instance::setMaterial(Material* m) {
    this->mat = m;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const 
{
    Intersection intersect;
    intersect.if_intersect = false;
    Ray inv_ray = Ray(this->inv_transform * ray.o, (this->inv_transform * ray.d).normalize());
    float previousBestDistance_new = (this->inv_transform * ray.getPoint(previousBestDistance) - inv_ray.o).length();

    intersect = this->instance->intersect(inv_ray, previousBestDistance_new);

    if(intersect.if_intersect && intersect.distance <= previousBestDistance)
    {
    //    Point hitpoint = ray.getPoint(intersect.distance);
    //    Vector normal  = (this->inv_transform.transpose()*intersect.normal()).normalize();
    //    return Intersection(intersect.distance, ray, nullptr, normal, hitpoint);
        Point hit_point_new = this->transform * intersect.hitPoint();
        float distance = (hit_point_new - ray.o).length();
        Vector normal_vector = (this->inv_transform.transpose() * intersect.normal()).normalize();
        if (dot(normal_vector, ray.d.normalize()) > 0)
            normal_vector = -1.0f * normal_vector;

        Intersection backTransformedIntersection = Intersection(distance, ray, nullptr, normal_vector, ray.getPoint(distance));
        return backTransformedIntersection;
    }
    
    return intersect;
}

BBox Instance::getBounds() const {
    BBox bound = this->instance->getBounds();
    Point maxP = this->transform * bound.max;
    Point minP = this->transform * bound.min;
    
    return BBox(min(maxP, minP), max(maxP, minP));
}

}