#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <limits>
#include <iostream>
#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

#define NO_CHILD -1
#define BINSIZE 8

namespace rt {

class BVHNode
{
public:
      BVHNode();
      BVHNode(const BBox& bound, const std::vector<Primitive*> p);

      int left_child;
      int right_child;
      BBox bound;
      std::vector<Primitive*> objects;

};

class BVH : public Group {
public:
    BVH(int max_objects = 3);
    //BVH();
	virtual ~BVH();

    BBox getBound(const Primitives& objs);
   // BBox getbox(const Primitive* s);
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = std::numeric_limits<float>::max()) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
    
private:
    int max_objects;
    std::vector<BVHNode> tree;
    BBox bound;
    BVHNode rootnode;

    void split(int tree_idx);
    float splitSAH(const BVHNode& parent, int axis, Point minP, Point maxP);
    Point getMiddlePoint(const Point& max, const Point& min);
    int* getAxis(const Point& max, const Point& min);
    void getsplit(int axis, float split_boundry, const Primitives parent_objs, BVHNode& left, BVHNode& right, BBox& left_b, BBox& right_b);
    void getcostParam(int axis, float split_boundry, const Primitives parent_objs, int& left_count, 
                   int& right_count, BBox& left_b, BBox& right_b);
};

}

#endif