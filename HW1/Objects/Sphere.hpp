#pragma once
#include <iostream>
#include "Vec3.hpp"
#include "Color.hpp"
#include "Material.hpp"
#include "Ray.hpp"

class Sphere {
    
    public:

        Material* material;
        Vec3* center;
        float r;

        Sphere(Material* material, Vec3* center, float r) {
        this -> material = material;
        this -> center = center;
        this -> r = r;
        }

        ~Sphere() {

        }

        inline double intersectRay(Ray* ray) {
            double A,B,C; //constants for the quadratic function	
            double delta;
            double t,t1,t2;

            float X = (ray -> e -> x - center -> x);
            float Y = (ray -> e -> y - center -> y);
            float Z = (ray -> e -> z - center -> z);

            float rDX = ray -> d -> x;
            float rDY = ray -> d -> y;
            float rDZ = ray -> d -> z;

            double tThreshold = ray -> minTValue;

            C = X * X + Y * Y + Z * Z- r * r;

            B = 2 * rDX * X + 
                2 * rDY * Y + 
                2 * rDZ * Z;

            A = rDX * rDX + 
                rDY * rDY + 
                rDZ * rDZ;
                
            delta = B * B - 4 * A * C;

            if (delta < 0.0) {
                return -1.0;
            }

            else if (delta == 0.0) {
                t = -B / (2*A);
            }
            else {
                delta = sqrt(delta);
                A = 2 * A;
                t1 = (-B + delta) / A;
                t2 = (-B - delta) / A;        
                t1 = std::min(t1,t2);
                if (t1 > tThreshold || FLOAT_EQ(t1, tThreshold)) {
                        t = t1;
                }
                else {
                        t = -1.0; // time returned negative. so it doesnt intersect
                }
            }
            return t;
        }
};