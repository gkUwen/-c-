#include <rt/groups/bvh.h>
#include <stack>
#include <limits>
#include <iostream>
#include <cstddef>
#include <assert.h>

namespace rt {

BVHNode::BVHNode()
{
    this->left_child = NO_CHILD;
    this->right_child = NO_CHILD;
    
}

BVHNode::BVHNode(const BBox& bound, const std::vector<Primitive*> p)
{
    this->bound = bound;
    this->objects = p;
    this->left_child = NO_CHILD;
    this->right_child = NO_CHILD;
}


BVH::BVH(int max_objects)
{
    this->max_objects = max_objects;
  
}

BVH::~BVH() {
    /* TODO */
}

void BVH::rebuildIndex() {
    this->bound = this->getBound(this->primitives);
    this->rootnode.bound = this->bound;
    this->rootnode.objects = this->primitives;  
   
    this->tree.push_back(this->rootnode);
    std::stack<int> node_idx;
    node_idx.push(0);
    int current_idx;
    while (node_idx.size() > 0)
    {
        current_idx = node_idx.top();
        node_idx.pop();

        if (tree[current_idx].objects.size() > this->max_objects)
        {
            
            this->split(current_idx);

            if(tree[current_idx].left_child != NO_CHILD)
            {
                node_idx.push(tree[current_idx].left_child);
            }
                
            if (tree[current_idx].right_child != NO_CHILD)
            {
                node_idx.push(tree[current_idx].right_child);
            }
               
        }
   
    }
}

BBox BVH::getBounds() const {
    return this->bound;
}

BBox BVH::getBound(const Primitives& objs)
{
    BBox result(Point(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), 
              Point(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()));
    
    for(int i = 0; i < objs.size(); i++)
    {
        BBox tmp = objs[i]->getBounds();
        result.extend(tmp);
    }
    return result;   
}

Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {

    Intersection intersect, intersect_temp;
    intersect.if_intersect = false; 
    int tree_idx;
    std::stack<int> node_idx;
    node_idx.push(0);
    std::pair<float, float> hitpoints;
    BVHNode current_node;

    while (node_idx.size() > 0)
    {
        tree_idx = node_idx.top();
        node_idx.pop();
        current_node = tree[tree_idx];

        if (!current_node.bound.isUnbound()){
            if(!current_node.objects.size())
            {
                hitpoints = current_node.bound.intersect(ray);

                if (hitpoints.first < hitpoints.second)
                {
                    node_idx.push(current_node.left_child);
                    node_idx.push(current_node.right_child);
                }
            }
            else 
            {
                for (int i = 0; i < current_node.objects.size(); i++)
                {

                    intersect_temp = current_node.objects[i]->intersect(ray, previousBestDistance);
                    if(intersect_temp.if_intersect 
                        && intersect_temp.distance > 0 
                        && intersect_temp.distance < previousBestDistance)
                    {
                        previousBestDistance = intersect_temp.distance;
                        intersect = intersect_temp;
                    }
                }
                
            }
        }
    }
    return intersect;
}

void BVH::add(Primitive* p) {
   this->primitives.push_back(p);
}


void BVH::setMaterial(Material* m) {
    for (int i = 0; i < this->tree.size(); i++){
        for(int j = 0; j < this->tree[i].objects.size(); j++)
        {
            this->tree[i].objects[j]->setMaterial(m);
        }
    }
}

void BVH::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::getsplit(int axis, float split_boundry, const Primitives parent_objs, BVHNode& left, 
                   BVHNode& right, BBox& left_b, BBox& right_b)
{
    for(int i = 0; i < parent_objs.size(); i++)
    {
        BBox tmp = parent_objs[i]->getBounds();
        if(!tmp.isUnbound())
        {
            Point m = this->getMiddlePoint(tmp.max, tmp.min);
            float value = m.getValue(axis);
            if(value > split_boundry)
            {
                left.objects.push_back(parent_objs[i]);
                left_b.extend(tmp);
            }
            else
            {
                right.objects.push_back(parent_objs[i]);
                right_b.extend(tmp);
            }
        }
    }
}

void BVH::getcostParam(int axis, float split_boundry, const Primitives parent_objs, int& left_count, 
                   int& right_count, BBox& left_b, BBox& right_b)
{
    for(int i = 0; i < parent_objs.size(); i++)
    {
        BBox tmp = parent_objs[i]->getBounds();
        if(!tmp.isUnbound())
        {
            Point m = this->getMiddlePoint(tmp.max, tmp.min);
            float value = m.getValue(axis);
            if(value > split_boundry)
            {
                left_count++;
                left_b.extend(tmp);
            }
            else
            {
                right_count++;
                right_b.extend(tmp);
            }
        }
    }
}

float BVH::splitSAH(const BVHNode& parent, int axis, Point minP, Point maxP)
{
    float candidates[BINSIZE];
    float tmp_cost;
    float best_cost = std::numeric_limits<float>::max();
    float cut_boundary;
    float bin_interval = (maxP.getValue(axis) - minP.getValue(axis))/BINSIZE;
   
    for(int i=0; i<BINSIZE; i++)
    {
        candidates[i] = minP.getValue(axis) + static_cast<float>(i)*bin_interval;
    }
    

    for (int i = 0; i < BINSIZE; i++)
    {
        BBox left_b = BBox::empty();
        BBox right_b = BBox::empty();
        int left_count=0;
        int right_count=0;
        
        getcostParam(axis, candidates[i], parent.objects, left_count, right_count, left_b, right_b);
        tmp_cost = (left_count*left_b.getArea() + right_count*right_b.getArea());
        
        if (tmp_cost < best_cost)
        {
            cut_boundary = candidates[i];
            best_cost = tmp_cost;
        }
    }
   return cut_boundary;
}

void BVH::split(int tree_idx){
    BVHNode parent = this->tree[tree_idx];
    Primitives parent_objs = parent.objects;
    Point maxP(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Point minP(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

    if(parent.bound.isUnbound())
    {
       for(int i = 0; i < parent_objs.size(); i++)
      {
          BBox tmp = parent_objs[i]->getBounds();
          if(!tmp.isUnbound())
          {
              maxP = max(maxP, tmp.max);
              minP = min(minP, tmp.min);
          }
      }
     
    }
    else
    {
        maxP = parent.bound.max;
        minP = parent.bound.min;
         
    }
   
   if(maxP!=minP)
   {
        Point middle = this->getMiddlePoint(maxP, minP);
        int* axis_ = this->getAxis(maxP, minP);
    
        for(int k = 0; k < 3; k++)
        {
            BVHNode left, right;
            BBox left_b = BBox::empty();
		    BBox right_b = BBox::empty();

            int axis = axis_[k];
            float split_value = middle.getValue(axis);
            //float split_value = this->splitSAH(parent, axis, minP, maxP); //use SAH split, comment in line 256 and 266 
            getsplit(axis, split_value, parent_objs, left, 
                   right, left_b, right_b);
            
            if(left.objects.size() > 0 && right.objects.size() > 0)
            {
                left.bound = left_b;
                this->tree[tree_idx].left_child = tree.size();
                this->tree.push_back(left);
            
                right.bound = right_b;
                this->tree[tree_idx].right_child = tree.size();   
                this->tree.push_back(right);
            
                this->tree[tree_idx].objects.clear();   
                break;
            }
           
        }
        delete []axis_;
    }

    
}

Point BVH::getMiddlePoint(const Point& max, const Point& min)
{
    return 0.5f*Point(min.x + max.x,
                     min.y + max.y,
                     min.z + max.z);
}

int* BVH::getAxis(const Point& max, const Point& min)
{
    float x = fabsf(max.x - min.x);
    float y = fabsf(max.y - min.y);
    float z = fabsf(max.z - min.z);
    float max_len = std::max(std::max(x,y),z);
    int* dims = new int[3];

    if(max_len == x)
    {
        dims[0] = x_axis;
        if(y>z)
        {
            dims[1] = y_axis;
            dims[2] = z_axis;
        }
        else
        {
            dims[1] = z_axis;
            dims[2] = y_axis;
        }
        return dims;
    }
    else if(max_len == y)
    {
        dims[0] = y_axis;
        if(x>z)
        {
            dims[1] = x_axis;
            dims[2] = z_axis;
        }
        else
        {
            dims[1] = z_axis;
            dims[2] = x_axis;
        }
        return dims;
    }
    else if(max_len == z)
    {
        dims[0] = z_axis;
        if(y>x)
        {
            dims[1] = y_axis;
            dims[2] = x_axis;
        }
        else
        {
            dims[1] = x_axis;
            dims[2] = y_axis;
        }
        return dims;
    }
    else
    {
        std::cout<<"error!!!!"<<std::endl;
        assert(false);
        return dims;
    }
    
  
}

}