#pragma once

#include "vector3f.h"

namespace fst
{
    class Ray;
    struct HitRecord;

    class Sphere
    {
    public:
        int texture_id;

        Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const;
        bool intersectShadowRay(const Ray& ray, float max_distance) const;

    private:
        math::Vector3f m_center;
        float m_radius;
        int m_material_id;
    };
}