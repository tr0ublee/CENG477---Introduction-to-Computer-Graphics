#include "texture.h"
#include <string>

namespace fst
{
  Texture::Texture() {}
  Texture::Texture(int width,
                   int height,
                   unsigned char *image,
                   std::string imageName,
                   std::string interpolation,
                   std::string decalMode,
                   std::string appearance)
      : m_width(width),
        m_height(height),
        m_image(image),
        m_imageName(imageName),
        m_interpolation(interpolation),
        m_decalMode(decalMode),
        m_appearance(appearance)
  {
  }

  // Texture::Texture(const Texture& text) {
    
  // }

  Texture::~Texture() {
  }

  math::Vector3f Texture::getUV(float u, float v) const {
    if (u > 1.0) {
      u = 1.0;
    }
    if (u < 0.0) {
      u = 0.0;
    }
    if (v > 1.0) {
      v = 1.0;
    }
    if (v < 0.0) {
      v = 0.0;
    }
    // round(i,j)
    float i = u * m_width;
    int imageX = std::round(i);
    float j = v * m_height ;
    int imageY = std::round(j);
    int index = m_width * imageX + imageY;
    index *= 3; 
    // (i, j)
    // calculat index
    // index = round(index)
    // image[index]
    fst::math::Vector3f color;
    color.x = m_image[index];
    color.y = m_image[index+1];
    color.z = m_image[index+2];
    return color;

  }
} // namespace fst