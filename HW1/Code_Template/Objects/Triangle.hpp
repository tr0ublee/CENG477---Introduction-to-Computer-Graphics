#pragma once
#include "Vec3.hpp"
#include "Face.hpp"
#include "Material.hpp"

class Triangle{

    public:

        Material* material;
        Face* indices;

        Triangle(Material* material, Face &indices);
        ~Triangle();

};