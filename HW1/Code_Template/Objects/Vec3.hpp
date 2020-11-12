// #pragma once
#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <math.h>       /* sqrt */
#define ABS(a) ((a)>0?(a):-1*(a))
#define SENSITIVITY 0.000001
#define FLOAT_EQ(a,b) (ABS(a - b) < SENSITIVITY)
#define FLOAT_G(a,b) (a > b)

class Vec3{

    public:
        float x;
        float y;
        float z;

        Vec3() {
            this -> x = 0;
            this -> y = 0;
            this -> z = 0;
        }

        Vec3(float x, float y, float z) {
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
            float x = this -> x + b.x;
            float y = this -> y + b.y;
            float z = this -> z + b.z;
            return Vec3(x, y, z);
        }

        inline Vec3 operator-(const Vec3 &b) {
            float x = this -> x - b.x;
            float y = this -> y - b.y;
            float z = this -> z - b.z;
            return Vec3(x, y, z);
        }
        inline Vec3 operator*(const float c) {
            float x = this -> x * c;
            float y = this -> y * c;
            float z = this -> z * c;
            return Vec3(x, y, z);
        }
        inline Vec3 operator*(const int c) {
            float x = this -> x * c;
            float y = this -> y * c;
            float z = this -> z * c;
            return Vec3(x, y, z);
        }
        inline Vec3 operator/(const float c) {
            float x = this -> x / c;
            float y = this -> y / c;
            float z = this -> z / c;
            return Vec3(x, y, z);
        }
        inline float dot(const Vec3 &b) {
            return this -> x * b.x + this -> y * b.y + this -> z * b.z;
        }
        inline bool operator==(const Vec3 &b) {
            float e = SENSITIVITY;
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
            float d;
            d = this -> getLength();
            this -> x = this -> x/d;
            this -> y = this -> y/d;
            this -> z = this -> z/d;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vec3& b) {
            return os << "X: " << b.x << " Y: " << b.y << " Z: " << b.z; 
        }
                
        inline float getLengthSquared() {
            float x = this -> x;
            float y = this -> y;
            float z = this -> z;
            return x*x + y*y + z*z;
        }
        inline float getLength() {
            float x = this -> x;
            float y = this -> y;
            float z = this -> z;
            return sqrt(x*x + y*y + z*z);
        }
        inline float distance(Vec3 &a) {
            return sqrt((a.x - x) * (a.x - x) + 
                        (a.y - y) * (a.y - y) + 
                        (a.z - z) * (a.z - z));
        }
        inline float distanceSquare(Vec3 &a) {
            return (a.x - x) * (a.x - x) + 
                        (a.y - y) * (a.y - y) + 
                        (a.z - z) * (a.z - z);
        }
};

#endif