#include "material.h"
#include <iostream>

namespace fst
{
    Material::Material(const math::Vector3f& ambient, const math::Vector3f& diffuse, const math::Vector3f& specular, const math::Vector3f& mirror, float phong_exponent)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_mirror(mirror)
        , m_phong_exponent(phong_exponent)
    {}

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal) const
    {
        auto diffuse = math::max(math::dot(normal, wi), 0.0f);
        auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);

        return m_specular * specular + m_diffuse * diffuse;
    }

    ColorInformation Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal, const HitRecord& hit_record, const Texture& txt) const
    {
        fst::math::Vector3f color = txt.getUV(hit_record.u, hit_record.v);
        auto diffuse = math::max(math::dot(normal, wi), 0.0f);
        auto specular = std::pow(math::max(math::dot(math::normalize(wo + wi), normal), 0.0f), m_phong_exponent);
        if (color.x > 255){
            color.x = 255;
        }
        if (color.y > 255) {
            color.y = 255;
        }
        if (color.z > 255) {
            color.z = 255;
        }
        ColorInformation retVal;
        if (txt.getDecalMode() == REPLACE_KD) {
            math::Vector3f C = color / 255.0;
            retVal.specular = m_specular * specular;
            retVal.diffuse = C * diffuse;
            return retVal;
        }
        else if (txt.getDecalMode() == BLEND_KD) {
            math::Vector3f C = color / 255;
            retVal.specular = m_specular * specular;
            retVal.diffuse = ((m_diffuse + C) / 2) * diffuse;
            return retVal;
        }
        else {
            retVal.specular = m_specular * specular;
            retVal.diffuse = color;
            return retVal;
        }
        // return m_specular * specular + m_diffuse * diffuse;
    }
}