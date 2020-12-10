#pragma once
#include <string>
#include "vector3f.h"

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
        fst::math::Vector3f getUV(float u, float v) const;
        ~Texture();
        unsigned char *m_image;

    private:
        int m_width;
        int m_height;
        std::string m_imageName;
        std::string m_interpolation;
        std::string m_decalMode;
        std::string m_appearance;
    };
}