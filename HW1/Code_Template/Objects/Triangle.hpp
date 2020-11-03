#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "Face.hpp"
#include "Material.hpp"

class Triangle{

    public:

        Material* material;
        Face* indices;
        Triangle(Material* material, Face &indices);
        ~Triangle();

};
#endif