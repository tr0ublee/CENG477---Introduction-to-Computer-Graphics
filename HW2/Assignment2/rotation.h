#pragma once

#include "matrix.h"
#include "vector3f.h"
#include "vector4f.h"

namespace fst
{
    class Rotation
    {
    public:
        float angle, rx, ry, rz;
        Rotation(float angle, float rx, float ry, float rz);
        Matrix getRotationMatrix();
    };
} // namespace fst
