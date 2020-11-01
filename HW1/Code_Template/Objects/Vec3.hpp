// #pragma once
#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <math.h>       /* sqrt */
#define ABS(a) ((a)>0?(a):-1*(a))
#define SENSITIVITY 0.000000001
#define FLOAT_EQ(a,b) (ABS(a - b) < SENSITIVITY)
#define FLOAT_G(a,b) (a > b)

class Vec3{

    public:
        double x;
        double y;
        double z;

        Vec3() {
            this -> x = 0;
            this -> y = 0;
            this -> z = 0;
        }

        Vec3(double x, double y, double z) {
            this -> x = x;
            this -> y = y;
            this -> z = z;
        }

        Vec3(const Vec3 &b) { 
            this -> x = b.x;
            this -> y = b.y;
            this -> z = b.z;
        }

        ~Vec3() {

        }

        inline Vec3 operator+(const Vec3 &b) {
            double x = this -> x + b.x;
            double y = this -> y + b.y;
            double z = this -> z + b.z;
            return Vec3(x, y, z);
        }

        inline Vec3 operator-(const Vec3 &b) {
            double x = this -> x - b.x;
            double y = this -> y - b.y;
            double z = this -> z - b.z;
            return Vec3(x, y, z);
        }
        inline Vec3 operator*(const double c) {
            double x = this -> x * c;
            double y = this -> y * c;
            double z = this -> z * c;
            return Vec3(x, y, z);
        }
        inline Vec3 operator*(const int c) {
            double x = this -> x * c;
            double y = this -> y * c;
            double z = this -> z * c;
            return Vec3(x, y, z);
        }
        inline double dot(const Vec3 &b) {
            return this -> x * b.x + this -> y * b.y + this -> z * b.z;
        }
        inline bool operator==(const Vec3 &b) {
            double e = SENSITIVITY;
            if ((ABS((this -> x - b.x)) < e) && (ABS((this -> y - b.y)) < e) && (ABS((this -> z - b.z)) < e)) { 
                return true;
            }
            return false;
        } 

        inline Vec3& operator=(const Vec3 &b) {
            this -> x = b.x;
            this -> y = b.y;
            this -> z = b.z;
            return *this;
        } 
        inline Vec3 cross(Vec3 &a, Vec3 &b) {
            Vec3 tmp;
            tmp.x = a.y*b.z-b.y*a.z;
            tmp.y = b.x*a.z-a.x*b.z;
            tmp.z = a.x*b.y-b.x*a.y;
            return tmp;
        }

        inline void normalize() {
            double d;
            d = this -> getLength();
            this -> x = this -> x/d;
            this -> y = this -> y/d;
            this -> z = this -> z/d;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vec3& b) {
            return os << "X: " << b.x << " Y: " << b.y << " Z: " << b.z; 
        }
                
        inline double getLengthSquared() {
            double x = this -> x;
            double y = this -> y;
            double z = this -> z;
            return x*x + y*y + z*z;
        }
        inline double getLength() {
            double x = this -> x;
            double y = this -> y;
            double z = this -> z;
            return sqrt(x*x + y*y + z*z);
        }
        inline double distance(Vec3 &a) {
            return sqrt((a.x - x) * (a.x - x) + 
                        (a.y - y) * (a.y - y) + 
                        (a.z - z) * (a.z - z));
        }
        inline double distanceSquare(Vec3 &a) {
            return (a.x - x) * (a.x - x) + 
                        (a.y - y) * (a.y - y) + 
                        (a.z - z) * (a.z - z);
        }
};

#endif