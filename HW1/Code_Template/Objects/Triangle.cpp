#include "Triangle.hpp"

Triangle::Triangle(Material* material, Face &indices) {
    this -> material = material;
    this -> indices = new Face(indices);
}

Triangle::~Triangle() {
    delete indices;
}