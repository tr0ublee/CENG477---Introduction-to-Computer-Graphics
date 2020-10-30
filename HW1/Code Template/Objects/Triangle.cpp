#include "Triangle.hpp"

Triangle::Triangle(Vec3 &a, Vec3 &b, Vec3 &c, Color &color) {
    this -> a = new Vec3(a);
    this -> b = new Vec3(b);
    this -> c = new Vec3(c);
    this -> color = new Color(color);
}

Triangle::~Triangle() {
    delete a;
    delete b;
    delete c;
    delete color;
}