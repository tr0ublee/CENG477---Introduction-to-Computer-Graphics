#include "Sphere.hpp"


Sphere::Sphere(Material* material, Vec3* center, double r) {
    this -> material = material;
    this -> center = center;
    this -> r = r;
}

Sphere::~Sphere() {

}