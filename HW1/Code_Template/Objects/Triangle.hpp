#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "Vec3.hpp"
#include "Face.hpp"
#include "Material.hpp"
#include "Ray.hpp"

class Triangle{

    public:

        Material* material;
        Face* indices;
        Vec3* normal;
        float intersectRay(Ray* ray);
        Triangle(Material* material, Face &indices);
        ~Triangle();

};
#endif