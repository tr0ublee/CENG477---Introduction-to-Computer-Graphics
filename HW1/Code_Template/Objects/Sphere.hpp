#pragma once
#include "Vec3.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "Ray.hpp"

class Sphere {
    
    public:

        Material* material;
        Vec3* center;
        float r;
        int intersectRay(Ray* ray);
        Sphere(Material* material, Vec3* center, float r);
        ~Sphere();
};