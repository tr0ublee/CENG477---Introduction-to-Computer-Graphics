#include "sphere.h"
#include "hit_record.h"
#include "ray.h"

namespace fst
{
    Sphere::Sphere(const math::Vector3f& center, float radius, int material_id, int texture_id, std::vector<fst::Rotation>& rot)
        : m_center(center)
        , m_radius(radius)
        , m_material_id(material_id)
        , texture_id(texture_id)
    {
        int size = rot.size();
        for (int i = 0; i < size; i++ ) {
            this -> rot.push_back(rot[i]);
        }
    }

    bool Sphere::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto sqrtf_x = sqrtf(x);
        auto distance = a - sqrtf_x;
        if (distance < 0.0f) {
            distance = a + sqrtf_x;
        }
        if (distance > 0.0f && distance < max_distance)
        {
            // Fill the intersection record.
            // intersection point is the distance.
            // rotate c, distance.

            math::Vector4f center(m_center.x, m_center.y, m_center.z, 1);
            math::Vector3f intersectionP = ray.getPoint(distance);
            math::Vector4f intersection4P(intersectionP.x, intersectionP.y, intersectionP.z, 1);
            Translation t(-center.x, -center.y, -center.z);
            Matrix tMatrix  = t.getTranslationMatrix();
            Matrix tBack = t.getInverseTranslationMatrix();
            // intersection4P = tMatrix * intersection4P;
            // center = tMatrix * center;
            for (int i = 0; i < rot.size(); i++) {
                Rotation r = rot[i]; 
                r.angle *= -1;
                Matrix m = r.getRotationMatrix();
                intersection4P =  m * intersection4P;
                center = m * center;
                // center = m  * center;
            }
            // intersection4P = tBack * intersection4P;
            math::Vector3f normal(intersection4P.x - center.x, intersection4P.y - center.y, intersection4P.z - center.z);
            normal = fst::math::normalize(normal);
            // calculate u, v
            float theta = acos((normal.y));
            float phi = atan2(normal.z, normal.x);
            hit_record.distance = distance;
            hit_record.normal = math::normalize(ray.getPoint(hit_record.distance) - m_center);
            hit_record.material_id = m_material_id;
            hit_record.texture_id = texture_id;
            hit_record.type = SPHERE;
            hit_record.u = (-phi + M_PI) / (2 * M_PI);
            hit_record.v = theta / M_PI;
            return true;
        }
        return false;
    }

    bool Sphere::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        //geometrical approach
        auto e = m_center - ray.get_origin();
        auto a = math::dot(e, ray.get_direction());
        auto x = m_radius * m_radius + a * a - math::dot(e, e);

        if (x < 0.0f)
        {
            return false;
        }

        auto distance = a - sqrtf(x);
        return distance > 0.0f && distance < max_distance;
    }
}