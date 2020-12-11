#pragma once
#include <string>
#include "vector3f.h"

enum Interpolation {
    NEAREST,
    BILINEAR
};
enum DecalMode {
    REPLACE_KD,
    BLEND_KD,
    REPLACE_ALL
};
enum Appearance {
    CLAMP,
    REPEAT
};

namespace fst
{
    class Texture
    {
    public:
        Texture();
        Texture(int width,
                int height,
                unsigned char *image,
                std::string imageName,
                std::string interpolation,
                std::string decalMode,
                std::string appearance);
        // Texture(const Texture& text);
        fst::math::Vector3f getUV(float u, float v, Interpolation type) const;
        Interpolation getInterpolation() const;
        DecalMode getDecalMode() const;
        Appearance getAppearance() const;
        ~Texture();
        unsigned char *m_image;

    private:
        int m_width;
        int m_height;
        std::string m_imageName;
        Interpolation m_interpolation;
        DecalMode m_decalMode;
        Appearance m_appearance;
    };
}