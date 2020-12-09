#include "scaling.h"
namespace fst
{
    Scaling::Scaling(float sx, float sy, float sz)
        : sx(sx)
        , sy(sy)
        , sz(sz)
    {}

    Matrix Scaling::getScalingMatrix () {
        Matrix scaleMatrix(sx, sy, sz, SCALE);
        return scaleMatrix;
    }
}
