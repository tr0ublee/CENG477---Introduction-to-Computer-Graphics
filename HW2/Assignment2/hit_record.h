#pragma once

#include "vector3f.h"

typedef enum obj_type {
    TRIANGLE,
    SPHERE
} OBJ;

namespace fst
{
    struct HitRecord
    {
        math::Vector3f normal;
        float distance;
        int material_id;
        int texture_id;
        float u;
        float v;
        OBJ type;

    };
}