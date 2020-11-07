#pragma once
#include "Vec3.hpp"

class Material{ 
    public:
        Vec3* ambientReflectance;
        Vec3* diffuseReflectance;
        Vec3* specularReflectance;
        Vec3* mirrorReflectance;
        int phongExponent;
        bool isMirror;

        Material(Vec3 &ambient, Vec3 &diff, Vec3 &spec, Vec3 &mirr, int phong);
        Material(const Material &a);
        ~Material();
};