#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <iostream>
#include <rt/cameras/camera.h>
#include <rt/integrators/integrator.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/color.h>
#include <core/scalar.h>
#include <core/julia.h>
#include <core/image.h>
#include <rt/renderer.h>
typedef unsigned int uint;


rt::RGBColor a1computeColor_for_bonus(int x, int y, int width, int height, rt::RGBColor c);
rt::RGBColor a1computeColor(uint x, uint y, uint width, uint height);
namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{
    this->cam = cam;
    this->integrator = integrator;
}

void Renderer::render(Image& img) {
    
    uint _width = img.width();
    uint _height = img.height();
    float ndcx = 0.0f, ndcy = 0.0f;
    float sscx = 0.0f, sscy = 0.0f;
   

    if(this->samples > 1)
    {
        for (uint px = 0; px < _width; px++)
        {
            for (uint py = 0; py < _height; py++)
            {
                RGBColor color(0.0,0.0,0.0);

                for(int i=0;i<this->samples;i++)
                {
                    ndcx = (px + random())/_width;
                    ndcy = (py + random())/_height;
                    sscx = ndcx*2.f - 1.f;
                    sscy = -ndcy*2.f + 1.f;
                    
                    color = color + this->integrator->getRadiance(this->cam->getPrimaryRay(sscx, sscy));
                }
                

                img(px, py) = color/this->samples;
            
            }
        }
    }
    else
    {
        for (uint px = 0; px < _width; px++)
        {
            for (uint py = 0; py < _height; py++)
            {
                ndcx = (px + 0.5)/_width;
                ndcy = (py + 0.5)/_height;
                sscx = ndcx*2.f - 1.f;
                sscy = -ndcy*2.f + 1.f;

                img(px, py) = this->integrator->getRadiance(this->cam->getPrimaryRay(sscx, sscy));
            
            }
        }
    }
    

    

}

}

namespace rt {

void Renderer::test_render1(Image& img) {
    
    uint _width = img.width();
    uint _height = img.height();

    for (uint px = 0; px < _width; px++){
        for (uint py = 0; py < _height; py++){
            img(px, py) = a1computeColor(px, py, _width, _height);
        }
    }
}

void Renderer::test_render_bonus(Image& img) {
    
    int _width = img.width();
    int _height = img.height();
    float ndcx = 0.0f, ndcy = 0.0f;
    float sscx = 0.0f, sscy = 0.0f;

    for (int px = 0; px < _width; px++){
        for (int py = 0; py < _height; py++){

            ndcx = (px + 0.5f)/_width;
            ndcy = (py + 0.5f)/_height;
            sscx = ndcx*2.f - 1.f;
            sscy = -ndcy*2.f + 1.f;
           
            RGBColor color = this->integrator->getRadiance(this->cam->getPrimaryRay(sscx,sscy));
            img(px, py) = a1computeColor_for_bonus(px, py, _width, _height, color);
        }
    }
}

}


rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
    uint _width = img.width();
    uint _height = img.height();
    float ndcx = 0.0f, ndcy = 0.0f;
    float sscx = 0.0f, sscy = 0.0f;
    
    for (uint px = 0; px < _width; px++){
        for (uint py = 0; py < _height; py++){
            ndcx = (px + 0.5f)/_width;
            ndcy = (py + 0.5f)/_height;
            sscx = ndcx*2.f - 1.f;
            sscy = -ndcy*2.f + 1.f;
            
            Ray _ray = cam->getPrimaryRay(sscx, sscy);
            RGBColor pixel_color = a2computeColor(_ray);

            img(px, py) = pixel_color;
        }
    }
}

}
