#pragma once
#include "Vec3.hpp"
#include "Color.hpp"

class Triangle{

    public:

        Vec3* a;
        Vec3* b;
        Vec3* c;
        Color* color;

        Triangle(Vec3 &a, Vec3 &b, Vec3 &c, Color &color);
        ~Triangle();

};