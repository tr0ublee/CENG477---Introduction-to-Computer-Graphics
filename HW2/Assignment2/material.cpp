#include "material.h"

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

    math::Vector3f Material::computeBrdf(const math::Vector3f& wi, const math::Vector3f& wo, const math::Vector3f& normal, const HitRecord& hit_record, const Texture& txt) const
    {
        auto diffuse = math::max(math::dot(normal, wi), 0.0f);
        // TODO: texture
        // const Texture* txt = &scene.textures[hit_record.texture_id];
        fst::math::Vector3f color = txt.getUV(hit_record.u, hit_record.v);
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
        return color;
        // return m_specular * specular + m_diffuse * diffuse;
    }
}