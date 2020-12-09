#include "rotation.h"

namespace fst
{
    Rotation::Rotation(float angle, float rx, float ry, float rz)
        : angle(angle)
        , rx(rx)
        , ry(ry)
        , rz(rz)
    {}

    Matrix Rotation::getRotationMatrix() {
        // normalize
        float len = sqrt(rx * rx + ry * ry + rz * rz);
        float normX = rx / len;
        float normY = ry / len;
        float normZ = rz / len;
        Matrix rotationMatrixX(normX, normY, normZ, X);
        Matrix rotationMatrixY(normX, normY, normZ, Y);
        Matrix rotationMatrixZ(normX, normY, normZ, Z, angle);
        Matrix inverseRotationMatrixX(normX, -normY, normZ, X);
        Matrix inverseRotationMatrixY(-normX, normY, normZ, Y);
        return inverseRotationMatrixX * inverseRotationMatrixY * rotationMatrixZ * rotationMatrixY * rotationMatrixX;
    }

}
