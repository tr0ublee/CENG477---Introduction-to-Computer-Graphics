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
        std::cout << "T1" << std::endl;
        std::cout<< T1 << std::endl;
        Matrix T2 = t.getInverseTranslationMatrix();
        std::cout << "T2" << std::endl;
        std::cout<< T2 << std::endl;

        return T2 * S * T1;
    }
}
