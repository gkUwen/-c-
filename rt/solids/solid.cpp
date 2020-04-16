#include <rt/solids/solid.h>
#include <rt/coordmappers/world.h>
#include <rt/coordmappers/plane.h>

namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material)
{
    this->material = material;
    if(texMapper == nullptr)
    {
        this->texMapper = new WorldMapper(Float4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    else 
    {
        this->texMapper = texMapper;
    }
}

}
