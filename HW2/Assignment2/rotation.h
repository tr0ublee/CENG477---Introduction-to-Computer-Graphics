#pragma once

#include "matrix.h"
#include "vector3f.h"
#include "vector4f.h"

namespace fst
{
    class Rotation
    {
    public:
        float angle, x, y, z;
        Rotation(float angle, float x, float y, float z);
        Matrix rotate(math::Vector3f& vertex);
    };
} // namespace fst
