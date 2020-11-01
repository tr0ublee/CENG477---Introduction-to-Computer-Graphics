#include "Ray.hpp"

Ray::Ray(size_t x, size_t y, Camera* cam) {
    Vec3 su, sv, s;
    this -> e = cam -> pos;
    su = *(cam -> u) * (x*(cam->pixelW) + (cam -> halfPixelW)); // OPTIMIZATION store su and recalculate when needed
    sv = *(cam -> v) * (y*(cam->pixelH) + (cam -> halfPixelH));
    s = *(cam->q) + su - sv;
    this -> d = new Vec3(s-(*e));
    eDynamic = false;
}

Ray::Ray(Vec3 &a, Vec3 &b) {
    this -> e = new Vec3(a);
    this -> d = new Vec3(b);
    eDynamic = true;
}
Ray::~Ray() {
    switch (eDynamic)
    {
    case true:
        delete e;
        delete d;
        break;
    
    default:
        delete d;
        break;
    }
}