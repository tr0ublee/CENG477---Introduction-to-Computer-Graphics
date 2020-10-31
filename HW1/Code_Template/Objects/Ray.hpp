#ifndef RAY_HPP
#define RAY_HPP
#include "Vec3.hpp"

class Ray{

    public:

        Vec3* a;
        Vec3* b;

        Ray(Vec3 &a, Vec3 &b);
        ~Ray();
};
#endif