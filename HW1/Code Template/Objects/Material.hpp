#pragma once
#include "Vec3.hpp"

class Material{ 

    public:
        Vec3* ambientReflectance;
        Vec3* diffuseReflectance;
        Vec3* specularReflectance;
        Vec3* mirrorReflectance;
        int phongExponent;

        Material(Vec3 &a, Vec3 &b, Vec3 &c, Vec3 &d, int phong);
        Material(const Material &a);
        ~Material();
};