#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <rt/solids/quad.h>
#include <rt/solids/disc.h>
#include <rt/solids/aabox.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/castingdist.h>
#include <rt/renderer.h>
#include <rt/ray.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->origin = origin;
    this->span1 = span1;
    // std::cout << this->span1.x << " " << " " <<this->span1.y << " " <<this->span1.z << " " << this->span1.lensqr() << "\n";
    this->span2 = span2;
    // std::cout << this->span2.x << " " << " " <<this->span2.y << " " <<this->span2.z << " " << this->span2.lensqr() << "\n";
    this->centroid = origin + span1*0.5f + span2*0.5f;
}

BBox Quad::getBounds() const {
    Point corner1 = Point(this->origin + this->span1);
    Point corner2 = Point(this->origin + this->span2);
    Point corner3 = Point(corner1 + this->span2);

    BBox bbox = BBox(min(origin, corner1), max(origin, corner1));

    bbox.extend(corner2);
    bbox.extend(corner3);

    return bbox;
}

Vector Quad::projection(const Vector& a, const Vector& b) const{

    Vector tmp;
    tmp.x = b.x * dot(a, b) * 1/b.lensqr();
    tmp.y = b.y * dot(a, b) * 1/b.lensqr();
    tmp.z = b.z * dot(a, b) * 1/b.lensqr();

    return tmp;
}

Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {

    bool check1 = true, check2 = true, check3 = true;

    Point corner1 = this->origin;
    Point corner2 = this->origin + this->span1;
    Point corner3 = corner2 + this->span2;
    Point corner4 = this->origin + this->span2;

    Vector normal_vector = cross(span1, span2).normalize();
    Intersection intersect;
    intersect.if_intersect = false;

    InfinitePlane plane = InfinitePlane(corner1, normal_vector, texMapper, material);
    intersect = plane.intersect(ray, previousBestDistance);

    if (intersect.if_intersect && intersect.distance <= previousBestDistance && intersect.distance > 0.001f){
        Vector norm_vec;
        
        if (dot(span1, Vector(ray.getPoint(intersect.distance) - this->origin)) >= 0 && projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).lensqr()/span1.lensqr() <= 1.0f &&
        dot(span2, Vector(ray.getPoint(intersect.distance) - this->origin)) >= 0 && projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).lensqr()/span2.lensqr() <= 1.0f){
            
            // std::cout << "00000000000000000 \n";
            if (dot(ray.d, normal_vector.normalize()) > 0) {
                norm_vec = -1.0f*normal_vector.normalize();
            }
            else{
                norm_vec = 1.0f*normal_vector.normalize();
            }
            intersect = Intersection(intersect.distance, ray, this, norm_vec , ray.getPoint(intersect.distance));
            
            Point tmp;
            tmp.z = 0.0f;//fabs(intersect.hitPoint().x - this->origin.x);
            tmp.x = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).length()/span1.length();//fabs(intersect.hitPoint().y - this->origin.y);
            tmp.y = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).length()/span2.length();//fabs(intersect.hitPoint().z - this->origin.z);

            //intersect.local() = tmp;
            intersect.localPoint = tmp;
            
            return intersect;
        }
        else{
            intersect.if_intersect = false;
            check1 = false;
        }

        
        if (dot(span1, Vector(ray.getPoint(intersect.distance) - this->origin)) <= 0 &&
        projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).lensqr() <=  projection(span2, span1).lensqr() && 
        dot(span2, Vector(ray.getPoint(intersect.distance) - this->origin)) >= 0 && projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).lensqr()/span2.lensqr() <= 1.0f){
            // std::cout << "1111111111111111111111111 \n ";
            
            if (dot(normal_vector.normalize(), ray.d) >= 0) {
                norm_vec = -1.0f*normal_vector.normalize();
            }
            else{
                norm_vec = 1.0f*normal_vector.normalize();
            }
            intersect = Intersection(intersect.distance, ray, this, norm_vec , ray.getPoint(intersect.distance));
            
            Point tmp;
            tmp.z = 0.0f;//fabs(intersect.hitPoint().x - this->origin.x);
            tmp.x = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).length()/span1.length();//fabs(intersect.hitPoint().y - this->origin.y);
            tmp.y = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).length()/span2.length();//fabs(intersect.hitPoint().z - this->origin.z);

            //intersect.local() = tmp;
            intersect.localPoint = tmp;
            
            return intersect;
        }
        else{
            intersect.if_intersect = false;
            check2 = false;
        }

        if (dot(span2, Vector(ray.getPoint(intersect.distance) - this->origin)) <= 0 &&
        projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).lensqr() <=  projection(span1, span2).lensqr() && 
        dot(span1, Vector(ray.getPoint(intersect.distance) - this->origin)) >= 0 && projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).lensqr()/span1.lensqr() <= 1.0f){
            // std::cout << "22222222222222222222222222 \n ";
            
            if (dot(normal_vector.normalize(), ray.d) >= 0) {
                norm_vec = -1.0f*normal_vector.normalize();
            }
            else{
                norm_vec = 1.0f*normal_vector.normalize();
            }
            intersect = Intersection(intersect.distance, ray, this, norm_vec , ray.getPoint(intersect.distance));

            Point tmp;
            tmp.z = 0.0f;//fabs(intersect.hitPoint().x - this->origin.x);
            tmp.x = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span1).length()/span1.length();//fabs(intersect.hitPoint().y - this->origin.y);
            tmp.y = projection(Vector(ray.getPoint(intersect.distance) - this->origin), span2).length()/span2.length();//fabs(intersect.hitPoint().z - this->origin.z);

            //intersect.local() = tmp;
            intersect.localPoint = tmp;
            
            return intersect;
        } 
        else
        {
            intersect.if_intersect = false;
            check3 = false;
        }
        
    }

    intersect.if_intersect = false;
    
    return intersect;
}

Solid::Sample Quad::sample() const {
    // http://stackoverflow.com/questions/240778/random-points-inside-a-polygon
	float u = random();
	float v = 1-u;
    Solid::Sample s;
    s.point = this->origin + u*this->span1 + v*this->span2;
    s.normal = cross(this->span1, this->span2).normalize();
	return s;
}

float Quad::getArea() const {
    return cross(this->span1, this->span2).length();

    // Point corner1 = this->origin;
    // Point corner2 = this->origin + this->span1;
    // Point corner3 = corner2 + this->span2;
    // Point corner4 = this->origin + this->span2;

    // return Triangle(corner1,corner2,corner3,nullptr,nullptr).getArea() + Triangle(corner2,corner3,corner4,nullptr,nullptr).getArea();
}

}

