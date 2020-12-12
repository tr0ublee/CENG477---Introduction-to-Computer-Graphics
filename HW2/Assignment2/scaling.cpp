#include "scaling.h"
namespace fst
{
    Scaling::Scaling(float sx, float sy, float sz)
        : sx(sx)
        , sy(sy)
        , sz(sz)
    {}

    Matrix Scaling::getScalingMatrix() {
        Matrix scaleMatrix(sx, sy, sz, SCALE);
        return scaleMatrix;
    }
    Matrix Scaling::getScalingMatrixWRTFixedPoint(math::Vector3f& fixedPoint) {
        Translation t(-fixedPoint.x, -fixedPoint.y, -fixedPoint.z);

        Matrix S = this -> getScalingMatrix();
        Matrix T1 = t.getTranslationMatrix();
        Matrix T2 = t.getInverseTranslationMatrix();
        return T2 * S * T1;
    }
}
