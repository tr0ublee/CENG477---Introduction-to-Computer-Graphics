#pragma once

#include "matrix.h"
#include "vector3f.h"
#include "vector4f.h"

namespace fst
{
    class Translation
    {
    public:
        float tx, ty, tz;
        Translation(float tx, float ty, float tz);
        math::Vector4f translate(math::Vector3f& vertex);
    };
} // namespace fst
