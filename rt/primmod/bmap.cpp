#include <rt/primmod/bmap.h>
#include <core/assert.h>
#include <rt/coordmappers/tmapper.h>
#include <rt/coordmappers/world.h>
#include <core/matrix.h>
//#include "intersection.h"

namespace rt {

BumpMapper::BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale)
{
    this->base = base;
    this->bumpmap = bumpmap;
    this->bv1 = bv1;
    this->bv2 = bv2;
    this->bv3 = bv3;
    this->vscale = vscale;
}

BBox BumpMapper::getBounds() const {
    return this->base->getBounds();
}

Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection intersection = this->base->intersect(ray, previousBestDistance);
        
    if (intersection.if_intersect)
    {
        Point texturePoint = this->base->texMapper->getCoords(intersection);

        Vector normal = intersection.normal().normalize();

        Point referenceOrigin = Point(0.0f, 0.0f, 0.0f);

        float dx = this->bumpmap->getColorDX(texturePoint).grayscale(); 
        float dy = this->bumpmap->getColorDY(texturePoint).grayscale();

        Matrix transformMat = Matrix(Float4(1.0f, 0.0f, 0.0f, -this->bv1.x),
                                     Float4(0.0f, 1.0f, 0.0f, -this->bv1.y),
                                     Float4(1.0f, 0.0f, 1.0f, -this->bv1.z),
                                     Float4(1.0f, 0.0f, 0.0f, 1.0f));

        Matrix intermediateMat = Matrix::system(this->bv2 - this->bv1, this->bv3 - this->bv1, cross(this->bv2 - this->bv1, this->bv3 - this->bv1)).transpose();

        transformMat = product(transformMat, intermediateMat);

        Vector wx = Vector(texturePoint.x, 0.0f, 0.0f).normalize();
        Vector wy = Vector(0.0f, texturePoint.y, 0.0f).normalize();

        Vector bumpNormal = normal + this->vscale*(dx*(cross(normal, wx.normalize()) + dy * cross(normal, wy.normalize())));

        // std::cout << dx << " " << dy << std::endl;

        Float4 newNormal = transformMat*Float4(bumpNormal);

        intersection.normal() = Vector(bumpNormal);
        auto prin = bumpNormal;
        //std::cout << prin.x << " " << prin.y << " " << prin.z << std::endl;
        return intersection;
    }

    return intersection;
}

void BumpMapper::setMaterial(Material* m) {
    this->base->setMaterial(m);
}

void BumpMapper::setCoordMapper(CoordMapper* cm) {
    this->base->setCoordMapper(cm);
}

}