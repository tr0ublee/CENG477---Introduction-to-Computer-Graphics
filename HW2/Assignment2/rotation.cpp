#include "rotation.h"

namespace fst
{
    Rotation::Rotation(float angle, float x, float y, float z)
        : angle(angle)
        , x(x)
        , y(y)
        , z(z)
    {}

    Matrix Rotation::rotate(math::Vector3f& vertex) {
        // normalize
        float len = sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z);
        float normX = vertex.x / len;
        float normY = vertex.y / len;
        float normZ = vertex.z / len;
        Matrix rotationMatrixX(normX, normY, normZ, X);
        Matrix rotationMatrixY(normX, normY, normZ, Y);
        Matrix rotationMatrixZ(normX, normY, normZ, Z, angle);
        return rotationMatrixZ * rotationMatrixY * rotationMatrixX;
    }

}
