#pragma once
#include "Material.hpp"
#include <vector>

class Mesh{
    public:

        Material* material;
        std::vector<std::vector<double>> Faces;

        Mesh(const Material &a, std::vector<std::vector<double>> &faces);
        ~Mesh();
};