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

        inline float intersectRay(Ray* ray) {
            float A,B,C; //constants for the quadratic function	
            float delta;
            float t,t1,t2;

            float X = (ray -> e -> x - center -> x);
            float Y = (ray -> e -> y - center -> y);
            float Z = (ray -> e -> z - center -> z);

            C = X * X + Y * Y + Z * Z- r * r;

            B = 2 * ray -> d -> x * X + 
                2 * ray -> d -> y * Y + 
                2 * ray -> d -> z * Z;

            A = ray -> d -> x * ray -> d -> x + 
                ray -> d -> y * ray -> d -> y + 
                ray -> d -> z * ray -> d -> z;
                
            delta = B * B - 4 * A * C;

            if (delta < 0.0f) {
                return -1.0;
            }

            else if (delta - 0.0f < 0.000001) {
                t = -B / (2*A);
            }
            else {
                delta = sqrt(delta);
                A = 2 * A;
                t1 = (-B + delta) / A;
                t2 = (-B - delta) / A;        
                t1 = std::min(t1,t2);
                if (t1 >= 0.0) {
                        t = t1;
                }
                else {
                        t = -1.0f; // time returned negative. so it doesnt intersect
                }
            }
            return t;
        }
};