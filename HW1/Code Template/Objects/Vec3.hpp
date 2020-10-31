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
        inline Vec3 operator+(const Vec3 &b);
        inline Vec3 operator-(const Vec3 &b);
        inline Vec3 operator*(const double c);
        inline double dot(const Vec3 &b);
        inline bool operator==(const Vec3 &b); 
        inline Vec3& operator=(const Vec3 &b); 
        inline Vec3 cross(Vec3 &a, Vec3 &b);
        inline void normalize();
        friend std::ostream& operator<<(std::ostream& os, const Vec3& b);
        inline double getLengthSquared();
        inline double getLength();
};

#endif