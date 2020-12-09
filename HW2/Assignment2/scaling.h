#pragma once

#include "matrix.h"
#include "vector3f.h"
#include "vector4f.h"
#include "translation.h"

namespace fst
{
    class Scaling
    {
    public:
        float sx, sy, sz;
        Scaling(float sx, float sy, float sz);
        Matrix getScalingMatrix();
        Matrix getScalingMatrixWRTFixedPoint(math::Vector3f& fixedPoint);
    };
}
