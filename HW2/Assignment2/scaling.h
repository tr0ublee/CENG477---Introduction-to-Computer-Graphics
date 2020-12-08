#pragma once

#include "matrix.h"
#include "vector3f.h"
#include "vector4f.h"

namespace fst
{
    class Scaling
    {
    public:
        float sx, sy, sz;
        Scaling(float sx, float sy, float sz);
        math::Vector4f scale(math::Vector3f& vertex);
    };
}
