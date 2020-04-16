#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <iostream>
#include <rt/coordmappers/world.h>


namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    this->v1 = vertices[0];
    this->v2 = vertices[1];
    this->v3 = vertices[2];
}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material):Solid(texMapper, material)
{
    float length1 = (v1 - Point(0,0,0)).length();
    float length2 = (v2 - Point(0,0,0)).length();
    float length3 = (v3 - Point(0,0,0)).length();
    float length[] = {length1, length2, length3};

    Point v_[] = {v1, v2, v3};

    float min = length[0];

    for(int i = 0; i < 3; i++){
        for (int j = i + 1; j < 3; j++)
        if (length[j] > length[i]){
            min = length[i];
            std::swap(v_[i], v_[j]);
            std::swap(length[i], length[j]);
        }
    }

    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
}

BBox Triangle::getBounds() const {
    Point maxPoint, minPoint;

    minPoint = min(this->v1, this->v2);
    maxPoint = max(this->v1, this->v2);

    minPoint = min(minPoint, this->v3);
    maxPoint = max(maxPoint, this->v3);

    return BBox(minPoint, maxPoint);

}

Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    
    /* Heavily inspired by "http://blackpawn.com/texts/pointinpoly/default.html"
    P = A + u * (C - A) + v * (B - A)       // Original equation
    (P - A) = u * (C - A) + v * (B - A)     // Subtract A from both sides
    v2 = u * v0 + v * v1                    // Substitute v0, v1, v2 for less writing
    
    We have two unknowns (u and v) so we need two equations to solve
    for them.  Dot both sides by v0 to get one and dot both sides by
    v1 to get a second.
    (v2) . v0 = (u * v0 + v * v1) . v0
    (v2) . v1 = (u * v0 + v * v1) . v1

    Distribute v0 and v1
    v2 . v0 = u * (v0 . v0) + v * (v1 . v0)
    v2 . v1 = u * (v0 . v1) + v * (v1 . v1)

    Now we have two equations and two unknowns and can solve one 
    equation for one variable and substitute into the other.  Or
    if you're lazy like me, fire up Mathematica and save yourself
    some handwriting.
    Solve[v2.v0 == {u(v0.v0) + v(v1.v0), v2.v1 == u(v0.v1) + v(v1.v1)}, {u, v}]
    u = ((v1.v1)(v2.v0)-(v1.v0)(v2.v1)) / ((v0.v0)(v1.v1) - (v0.v1)(v1.v0))
    v = ((v0.v0)(v2.v1)-(v0.v1)(v2.v0)) / ((v0.v0)(v1.v1) - (v0.v1)(v1.v0))

    Compute vectors        
    v0 = C - A
    v1 = B - A
    v2 = P - A

    Compute dot products
    dot00 = dot(v0, v0)
    dot01 = dot(v0, v1)
    dot02 = dot(v0, v2)
    dot11 = dot(v1, v1)
    dot12 = dot(v1, v2)

    Compute barycentric coordinates
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01)
    u = (dot11 * dot02 - dot01 * dot12) * invDenom
    v = (dot00 * dot12 - dot01 * dot02) * invDenom

    Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1)

    */

    //Intersection intersection;
    //intersection.if_intersect = false;

    //Vector v0 = this->v3 - this->v1;
    //Vector v1 = this->v2- this->v1;
    ////Vector v2 = this->v3 - this->v1;
    //Vector normal_vector = cross(v0, v1).normalize();
    //    
    //InfinitePlane plane(this->v2, normal_vector, texMapper, material);

    //intersection = plane.intersect(ray);

    //if (dot(ray.d, intersection.normal()) > 0){
    //    intersection.normal() = -1.0f*normal_vector;
    //}

    //Point hit_point = intersection.hitPoint();
    //Point hit_point_barycentric;

    //Vector v2 = hit_point - this->v1;

    //float dot00 = dot(v0, v0);
    //float dot01 = dot(v0, v1);
    //float dot02 = dot(v0, v2);
    //float dot11 = dot(v1, v1);
    //float dot12 = dot(v1, v2);
    //// float dot12_12 = dot(v1v2, v1v2);
    //// float dot12_13 = dot(v1v2, v1v3); 
    //// float dot12_1hitpoint = dot(v1v2, v1hpoint); 
    //// float dot13_13 = dot(v1v3, v1v3);
    //// float dot13_1hitpoint = dot(v1v3, v1hpoint);

    //float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    //float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    //float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    ////float Denom = 1/(dot12_12*dot13_13 - dot12_13*dot12_13);

    //hit_point_barycentric.z = u;//(dot13_13*dot12_1hitpoint - dot12_13*dot13_1hitpoint)*Denom;
    //hit_point_barycentric.y = v;//(dot12_12*dot13_1hitpoint - dot12_13*dot12_1hitpoint)*Denom;
    //hit_point_barycentric.x = 1.0f - u - v;// hit_point_barycentric.x - hit_point_barycentric.y;

    //if ((hit_point_barycentric.x >= 0 && hit_point_barycentric.x <= 1) &&
    //    (hit_point_barycentric.y >= 0 && hit_point_barycentric.y <= 1) &&
    //    (hit_point_barycentric.z >= 0 && hit_point_barycentric.z <= 1)){

    //    //intersection.local() = hit_point_barycentric;
    //    intersection.localPoint = hit_point_barycentric;

    //   // std::cout << hit_point_barycentric.x << "**" << hit_point_barycentric.y << "**" << hit_point_barycentric.z << std::endl;
    //       return intersection;
    //}

    //intersection.if_intersect = false; 


    //
    //return intersection;




     //Intersection intersection;
     //intersection.if_intersect = false;
    
     //Vector v1v2 = this->v2 - this->v1;
     //Vector v1v3 = this->v3 - this->v1;
     //Vector v2v3 = this->v3 - this->v2;
     //Vector normal_vector = cross(v1v2, v1v3).normalize();
    

     //InfinitePlane plane(this->v2, normal_vector, texMapper, material);
     //intersection = plane.intersect(ray, previousBestDistance);

     //if (dot(ray.d, intersection.normal()) > 0){
     //    intersection.normal() = -1.0f*normal_vector;
     //}

     //Point hit_point = intersection.hitPoint();
     //Point hit_point_barycentric;
     ////Vector v1hpoint = hit_point - this->v1;

     //if (intersection.if_intersect && intersection.distance < previousBestDistance){  

     //    Vector cv1 = this->v1 - hit_point;
     //    Vector cv2 = this->v2 - hit_point;
     //    Vector cv3 = this->v3 - hit_point;

     //    float l1 = v1v2.length();// sqrt(dot12_12);
     //    float l2 = v1v3.length(); // sqrt(dot13_13);
     //    float l3 = v2v3.length(); // sqrt(dot23_23);
     //    float a1 = cv1.length(); // sqrt(dotcv1);
     //    float a2 = cv2.length(); // sqrt(dotcv2);
     //    float a3 = cv3.length(); // sqrt(dotcv3);
     //    
     //    float p  = (l1+l2+l3)*0.5;
     //    float p1 = (l1+a1+a2)*0.5;
     //    float p2 = (l2+a1+a3)*0.5;
     //    float p3 = (l3+a2+a3)*0.5;
    
     //    float whole_area = sqrt(p*(p-l1)*(p-l2)*(p-l3));

     //    hit_point_barycentric.z = (sqrt(p1*(p1-l1)*(p1-a1)*(p1-a2)))/whole_area;
     //    hit_point_barycentric.y = (sqrt(p2*(p2-l2)*(p2-a1)*(p2-a3)))/whole_area;
     //    hit_point_barycentric.x = (sqrt(p3*(p3-l3)*(p3-a2)*(p3-a3)))/whole_area;

     //    if ((hit_point_barycentric.x >= 0 && hit_point_barycentric.x <= 1) &&
     //        (hit_point_barycentric.y >= 0 && hit_point_barycentric.y <= 1) &&
     //        (hit_point_barycentric.z >= 0 && hit_point_barycentric.z <= 1)){
     //           
     //            Intersection __intersection = Intersection(intersection.distance, ray, this, normal_vector.normalize(), ray.getPoint(intersection.distance));
     //           // __intersection.local () = hit_point_barycentric;
     //            __intersection.localPoint = hit_point_barycentric;
     //            return __intersection;
     //    }else{       
     //        intersection.if_intersect = false;   
     //    }
     //}

     //return intersection;



     Intersection inter;
     inter.if_intersect = false;
       
     float det, invdet;
     float u, v, t;
     Vector p, q, r, edge1, edge2, normal;
     Point local;

     edge1 = v2 - v1;
     edge2 = v3 - v1;
    
     p = cross(ray.d, edge2);
     det = dot(edge1, p);

     if(fabs(det) < 0.001f){
          return inter;
     }

     invdet = 1.0f / det;
     r = ray.o - v1;
     u = dot(r, p)*invdet;
     if(u<0 || u>1){
         return inter;
     }

     q = cross(r, edge1);
     v = dot(ray.d, q)*invdet;
     if(v < 0 || u + v > 1){
         return inter;
     }

     t = dot(edge2, q)*invdet;

     if(t > previousBestDistance || t < 0){
         return inter;
     }

     normal = cross(edge1, edge2).normalize();
     local = Point(1-u-v, u, v);
     inter.localPoint = local;
     
     return Intersection(t, ray, this, normal, local);
}

Solid::Sample Triangle::sample() const {
    // http://math.stackexchange.com/questions/18686/uniform-random-point-in-triangle
    float r1 = random();
    float r2 = random();
    float r1_sq = sqrt(r1);

    Solid::Sample s;
    s.point = (1 - r1_sq) * this->v1 + (r1_sq*(1 - r2)) * this->v2 + (r2*r1_sq) * this->v3;
    s.normal = cross(this->v2-this->v1, this->v3-this->v1).normalize();
    
    return s;
}

float Triangle::getArea() const {
    
    return cross(this->v2 - this->v1, this->v3 - this->v1).length()*0.5f;
    // Vector v1v2 = this->v2 - this->v1;
    //  Vector v1v3 = this->v3 - this->v1;
    //  Vector v2v3 = this->v3 - this->v2;

    //     float l1 = v1v2.length();// sqrt(dot12_12);
    //     float l2 = v1v3.length(); // sqrt(dot13_13);
    //     float l3 = v2v3.length(); // sqrt(dot23_23);

        
    //     float p  = (l1+l2+l3)*0.5;

    
    //     return sqrt(p*(p-l1)*(p-l2)*(p-l3));
}


}
