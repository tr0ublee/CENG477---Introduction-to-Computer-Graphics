#ifndef RAY_HPP
#define RAY_HPP
#include "Vec3.hpp"
#include "Camera.hpp"

class Ray{

    public:

        Vec3* e;
        Vec3* d;
        bool eDynamic;

        Ray(Vec3 &a, Vec3 &b);
        Ray (size_t x, size_t y, Camera* cam);
        ~Ray();
};
#endif