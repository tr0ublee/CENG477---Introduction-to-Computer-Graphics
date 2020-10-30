#include "Ray.hpp"


Ray::Ray(Vec3 &a, Vec3 &b) {
    this -> a = new Vec3(a);
    this -> b = new Vec3(b);
}
Ray::~Ray() {
    delete a;
    delete b;
}