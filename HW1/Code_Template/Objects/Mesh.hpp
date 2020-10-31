#pragma once
#include "Material.hpp"
#include <vector>
#include "Face.hpp"
class Mesh{
    public:

        Material* material;
        std::vector<Face*> Faces;
        size_t numOfFaces;
        Mesh(Material *a, std::vector<Face> &faces, size_t faceSize);
        ~Mesh();
};