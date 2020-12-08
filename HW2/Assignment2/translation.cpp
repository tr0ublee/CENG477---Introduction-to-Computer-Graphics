#include "translation.h"

namespace fst
{
    Translation::Translation(float tx, float ty, float tz)
        : tx(tx), ty(ty), tz(tz)
    {
    }
    /**
     * (T(S(TP)))
     * 
     * translate(-rotate(translate());
     * -T * S * T * P
     * */
    math::Vector4f Translation::translate(math::Vector3f& vertex3f) {
        Matrix translationMatrix(tx, ty, tz, TRANSLATE);
        math::Vector4f vertex4f(vertex3f.x, vertex3f.y, vertex3f.z, 1);
        return translationMatrix * vertex4f;
    }
} // namespace fst
