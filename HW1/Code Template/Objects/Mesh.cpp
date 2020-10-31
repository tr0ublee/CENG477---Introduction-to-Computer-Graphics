#include "Mesh.hpp"

Mesh::Mesh(const Material &a, std::vector<std::vector<double>> &faces) {
    material = new Material(a);
    size_t rows = faces.size();
    for(size_t i = 0; rows;i++) {
        size_t cols = faces[i].size();
        for(int j = 0; j < cols; j++) {
            Faces[i].push_back(faces[i][j]);
        }
    }
}

Mesh::~Mesh() {
    delete material;
}