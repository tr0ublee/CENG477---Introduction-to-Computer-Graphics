#include "Mesh.hpp"

Mesh::Mesh(Material *a, std::vector<Face> &faces, size_t faceSize) {
    material = a;
    numOfFaces = faceSize;
    for(size_t i = 0; i < numOfFaces; i++) {
        Face* pushed = new Face(faces[i].v0, faces[i].v1, faces[i].v2); // maybe do the allocation in the scene constructor?
        Faces.push_back(pushed);
    }
}

Mesh::~Mesh() {
    for(size_t i = 0; i < numOfFaces; i++) {
        delete Faces[i];
    }
}