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
        Matrix out(rx, ry, rz, ROTATE, angle);
        return out;
    }

}
