#ifndef FACE_HPP
#define FACE_HPP
#include "Vec3.hpp"
#include "Ray.hpp"

class Face{
    public:
    
        Vec3* v0;
        Vec3* v1;
        Vec3* v2;
        Vec3* normal;

        float intersectRay(Ray* ray);
        Face(Vec3* v0, Vec3* v1, Vec3* v2);
        ~Face();
        Face(const Face &copy);
};

#endif