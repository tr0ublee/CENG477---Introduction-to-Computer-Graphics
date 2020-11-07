#include "Ray.hpp"

Ray::Ray(size_t x, size_t y, Camera* cam, Vec3 &sv, bool isPrimary) {
    Vec3 su, s;
    this -> e = cam -> pos;
    su = *(cam -> u) * (x*(cam->pixelW) + (cam -> halfPixelW));
    s = *(cam->q) + su - sv;
    this -> d = new Vec3(s-(*e));
    if (isPrimary) {
        minTValue = 1.0f;
    } else {
        minTValue = 0.0f;
    }
    eDynamic = false;
}

Ray::Ray(Vec3 &a, Vec3 &b, bool isPrimary) {
    this -> e = new Vec3(a);
    this -> d = new Vec3(b);
    if (isPrimary) {
        minTValue = 1.0f;
    } else {
        minTValue = 0.0f;
    }
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