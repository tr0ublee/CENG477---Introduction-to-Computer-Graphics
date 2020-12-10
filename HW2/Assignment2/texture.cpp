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
        m_imageName(imageName)
  {
    if (interpolation == "nearest") {
      m_interpolation = NEAREST;
    } else {
      m_interpolation = BILINEAR;
    }
    
    if (decalMode == "replace_kd") {
      m_decalMode = REPLACE_KD;
    } else if (decalMode == "blend_kd") {
      m_decalMode = BLEND_KD;
    } else {
      m_decalMode = REPLACE_ALL;
    }

    if (appearance == "clamp") {
      m_appearance = CLAMP;
    } else {
      m_appearance = REPEAT;
    }
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
    float i = u * (m_width);
    int imageX = std::round(i);
    if (imageX == m_width){
      imageX = m_width - 1;
    }
    float j = v * (m_height) ;
    int imageY = std::round(j);
    if (imageY == m_height){
      imageY = m_height - 1;
    }
    int index = m_width * imageY + imageX;
    index *= 3; 
    fst::math::Vector3f color;
    color.x = m_image[index];
    color.y = m_image[index+1];
    color.z = m_image[index+2];
    return color;

  }
  Interpolation Texture::getInterpolation() const {
    return m_interpolation;
  }
  DecalMode Texture::getDecalMode() const {
    return m_decalMode;
  }
  Appearance Texture::getAppearance() const {
    return m_appearance;
  }
} // namespace fst