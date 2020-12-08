#include "scaling.h"
namespace fst
{
    Scaling::Scaling(float sx, float sy, float sz)
        : sx(sx)
        , sy(sy)
        , sz(sz)
    {}

    math::Vector4f Scaling::scale (math::Vector3f& vertex3f) {
        Matrix scaleMatrix(sx, sy, sz, SCALE);
        math::Vector4f vertex4f(vertex3f.x, vertex3f.y, vertex3f.z, 1);
        return scaleMatrix * vertex4f;
    }
}
