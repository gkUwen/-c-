#include <rt/textures/imagetex.h>
#include <core/assert.h>
#include <core/image.h>

#include <stdio.h>

namespace rt {

ImageTexture::ImageTexture()
{
    /* TODO */
}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
{
    this->bh = bh;
    this->intp = i;
    this->filename = filename;
    this->image.readPNG(filename);
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
{
    this->image = image;
    this->bh = bh;
    this->intp = i;
}

RGBColor ImageTexture::getColor(const Point& coord) {
    
    float u, v; 
    float lu, lv; 
    float fu, fv; 

    //Border Handling
    int _lu = floor(coord.x);
    int _lv = floor(coord.y);

    //Fractional Coordinates
    float tu = fabs(coord.x - _lu);
    float tv = fabs(coord.y - _lv);

    if (bh == REPEAT); //done in above
    
    if (bh == MIRROR){
        if(abs(_lu) % 2 == 1) tu = 1 - tu;
        if(abs(_lv) % 2 == 1) tv = 1 - tv;
    }

    if (bh == CLAMP){
        if(coord.x < 0) tu = 0;
        else if(coord.x > 1) tu = 1;
        else tu = coord.x;

        if(coord.y < 0) tv = 0;
        else if(coord.y > 1) tv = 1;
        else tv = coord.y;
    }
    
    if (intp == NEAREST) {
        u = tu * image.width();
        v = tv * image.height();
        
        lu = std::fmin(floor(u), image.width() - 1);
        lv = std::fmin(floor(v), image.height() - 1);

        return image(lu, lv);
    }

    if (intp == BILINEAR){
        u = tu * (image.width() - 1);
        v = tv * (image.height() - 1);

        fu = u - floor(u);
        fv = v - floor(v);


        return ((1 - fu)*(1 - fv)*image(floor(u), floor(v)) 
                + ((1 - fu)*fv*image(floor(u), std::fmin(floor(v) + 1, image.height() - 1)))
                + (fu*(1 - fv)*image(std::fmin(floor(u) + 1, image.width() - 1), floor(v) ))
                + (fu*fv* image(std::fmin(floor(u) + 1, image.width() - 1), std::fmin(floor(v) + 1, image.height() - 1))));
    }

}

RGBColor ImageTexture::getColorDX(const Point& coord) {
    RGBColor first = getColor(Point(coord.x, coord.y, 0.0f));
    RGBColor second = getColor(Point(coord.x + (1.0f/ image.width()), coord.y, 0.0f));
    
    return second - first;
}

RGBColor ImageTexture::getColorDY(const Point& coord) {
    RGBColor first = getColor(Point(coord.x, coord.y, 0.0f));
    RGBColor second = getColor(Point(coord.x , coord.y + (1.0f/ image.height()), 0.0f));
    
    return second - first;
}

}
