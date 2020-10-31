#pragma once
#include "Vec3.hpp"
#include "Color.hpp"
#include "Material.hpp"

class Sphere {
    
    public:

        Material* material;
        Vec3* center;
        double r;
        
        Sphere(Material* material, Vec3* center, double r);
        ~Sphere();
};