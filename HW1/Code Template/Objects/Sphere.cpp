#include "Sphere.hpp"

Sphere::Sphere(Vec3 &a, double r, Color &c) {
    center = new Vec3(a);
    this -> r = r;
    color = new Color(c);
}

Sphere::~Sphere() {
    delete center;
    delete color;
}