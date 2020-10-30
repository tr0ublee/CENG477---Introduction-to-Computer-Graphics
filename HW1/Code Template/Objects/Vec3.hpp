// #pragma once
#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <math.h>       /* sqrt */
#define ABS(a) ((a)>0?(a):-1*(a))

class Vec3{

    public:
        double x;
        double y;
        double z;

        Vec3();
        Vec3(double x, double y, double z);
        Vec3(const Vec3 &b);
        ~Vec3();
        Vec3 operator+(const Vec3 &b);
        Vec3 operator-(const Vec3 &b);
        Vec3 operator*(const double c);
        double dot(const Vec3 &b);
        bool operator==(const Vec3 &b); 
        void operator=(const Vec3 &b); 
        Vec3 cross(Vec3 &a, Vec3 &b);
        void normalize();
        friend std::ostream& operator<<(std::ostream& os, const Vec3& b);
        double getLengthSquared();
        double getLength();
};

#endif