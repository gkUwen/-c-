#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
    NOT_IMPLEMENTED;//
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    return RGBColor(this->r+c.r, this->g+c.g, this->b+c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    return RGBColor(this->r-c.r, this->g-c.g, this->b-c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    return RGBColor(this->r*c.r, this->g*c.g, this->b*c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
    return this->r==c.r && this->g==c.g && this->b==c.b;
}

bool RGBColor::operator != (const RGBColor& b) const {
    return !this->operator==(b);
}

RGBColor RGBColor::clamp() const {   

    RGBColor channel = RGBColor::rep(0.0f);

    channel.r = RGBColor::r;
    channel.g = RGBColor::g;
    channel.b = RGBColor::b;

    if (channel.r > 1.f) channel.r = 1.f;
    if (channel.g > 1.f) channel.g = 1.f;
    if (channel.b > 1.f) channel.b = 1.f;

    if (channel.r < 0.f) channel.r = 0.f;
    if (channel.g < 0.f) channel.g = 0.f;
    if (channel.b < 0.f) channel.b = 0.f;

    return channel;
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator * (const RGBColor& c, float scalar) {
    return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator / (const RGBColor& c, float scalar) {
    return RGBColor(c.r/scalar, c.g/scalar, c.b/scalar);
}

float RGBColor::grayscale() const
	{
		return (this->r + this->g + this->b) / 3;
	}

}
