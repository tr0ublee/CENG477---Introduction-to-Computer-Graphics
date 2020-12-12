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
        float a = rx / len;
        float b = ry / len;
        float c = rz / len;
        Matrix rotationMatrixX(a, b, c, X);
        Matrix rotationMatrixY(-a, b, c, Y);
        Matrix rotationMatrixZ(a, b, c, Z, angle);
        Matrix inverseRotationMatrixX(a, -b, c, X);
        Matrix inverseRotationMatrixY(a, b, c, Y);
        Matrix out = inverseRotationMatrixX * inverseRotationMatrixY * rotationMatrixZ * rotationMatrixY * rotationMatrixX;
        return out;
    }

}
