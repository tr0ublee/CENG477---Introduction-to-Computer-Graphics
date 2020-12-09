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
        Matrix getTranslationMatrix();
        Matrix getInverseTranslationMatrix();
    };
} // namespace fst
