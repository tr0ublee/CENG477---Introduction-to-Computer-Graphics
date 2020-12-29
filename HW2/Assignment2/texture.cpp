#include "texture.h"
#include <string>
#include <iostream>

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
    if (m_appearance == CLAMP) {
      if (u < 0) {
        u = 0;
      } else if (u > 1) {
        u = 1;
      }
      if (v < 0) {
        v = 0;
      } else if (v > 1) {
        v = 1;
      }
    } else {
      // REPEAT
      u = u - floor(u);
      v = v - floor(v);
    }
   
    float i = u * (m_width);
    float j = v * (m_height);
    if (m_interpolation == BILINEAR) {
      int p = floor(i);
      int q = floor(j);
      if (p >= m_width - 1){
        p = m_width - 2;
      }
      if (q >= m_height - 1){
        q = m_height - 2;
      }
      float dx = i - p;
      float dy = j - q;
      int index0 = 3 * (m_width * q + p);
      int index1 = 3 * (m_width * q + p+1);
      int index2 = 3 * (m_width * (q+1) + p);
      int index3 = 3 * (m_width * (q+1) + p+1);
      fst::math::Vector3f color;
      color.x = m_image[index0] * (1-dx) * (1-dy) + m_image[index1] * dx* (1-dy) + m_image[index2] * (1-dx) * dy  + m_image[index3] * dx * dy;
      color.y = m_image[index0+1] * (1-dx) * (1-dy) + m_image[index1+1] * dx* (1-dy) + m_image[index2+1] * (1-dx) * dy  + m_image[index3+1] * dx * dy;
      color.z = m_image[index0+2] * (1-dx) * (1-dy) + m_image[index1+2] * dx* (1-dy) + m_image[index2+2] * (1-dx) * dy  + m_image[index3+2] * dx * dy;
      return color;

    } else {
      //
      float i = u * (m_width);
      float j = v * (m_height);
      int imageX = std::round(i);
      int imageY = std::round(j);
      if (imageX >= m_width){
        imageX = m_width - 1;
      }
      if (imageY >= m_height){
        imageY = m_height - 1;
      }
      int index = 3 * (m_width * imageY + imageX);
      fst::math::Vector3f color;
      color.x = m_image[index];
      color.y = m_image[index+1];
      color.z = m_image[index+2];
      return color;
    }
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