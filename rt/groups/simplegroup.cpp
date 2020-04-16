#include <rt/groups/simplegroup.h>
#include <rt/bbox.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
   BBox tmp_box = BBox::empty();
   for(int i = 0; i < this->objects.size(); i++)
   {
       tmp_box.extend(this->objects[i]->getBounds());
   }

   return tmp_box;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
    
     Intersection intersect, current;
     intersect.if_intersect = false;
     
     for(int i=0; i < this->objects.size(); i++)
     {
         current = this->objects[i]->intersect(ray, previousBestDistance);
         
         if(current.if_intersect)//&& current.distance < pre_bestDistance)
         {
            previousBestDistance = current.distance;
             intersect = current;
         }
     }
    return intersect;

}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    this->objects.push_back(p);
}

void SimpleGroup::setMaterial(Material* m) {
    for (int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setMaterial(m);
    }
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    for (int i = 0; i < this->objects.size(); i++) {
        this->objects[i]->setCoordMapper(cm);
    }
}

}
