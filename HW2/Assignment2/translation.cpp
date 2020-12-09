#include "translation.h"

namespace fst
{
    Translation::Translation(float tx, float ty, float tz)
        : tx(tx), ty(ty), tz(tz)
    {
    }

    Matrix Translation::getTranslationMatrix() {
        Matrix translationMatrix(tx, ty, tz, TRANSLATE);
        return translationMatrix;
    }

    Matrix Translation::getInverseTranslationMatrix() {
        Matrix translationMatrix(-tx, -ty, -tz, TRANSLATE);
        return translationMatrix;
    }
} // namespace fst
