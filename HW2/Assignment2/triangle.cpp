#include "hit_record.h"
#include "triangle.h"
#include "ray.h"

namespace fst
{
    Triangle::Triangle(const math::Vector3f& v0, const math::Vector3f& edge1, const math::Vector3f& edge2, int texture_id, std::vector<math::Vector2f>& texMap)
        : m_v0(v0)
        , m_edge1(edge1)
        , m_edge2(edge2)
        , m_normal(math::normalize(math::cross(edge1, edge2)))
        , texture_id(texture_id)
        , texMap(texMap)
    {}

    bool Triangle::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)  // that is u
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f) // that is v
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        if (distance > 0.0f && distance < max_distance)
        {
            //Fill the intersection record.
            hit_record.normal = m_normal;
            hit_record.distance = distance;
            hit_record.type = TRIANGLE;
            hit_record.u = w1;
            hit_record.v = w2;

            /** If has a texture, apply Barycentric and replace (u,v) **/
            if (hit_record.texture_id >= 0 && texMap.size() >= 3) {
                double  a,b,c,d,e,f,g,h,i,j,k,l;
                double alpha,beta,gamma,t;
                
                double eimhf,gfmdi,dhmeg,akmjb,jcmal,blmkc;

                double M;
                
                double dd;
                math::Vector3f ma,mb,mc;

                ma = m_v0;
                mb = m_v0 + m_edge1;
                mc = m_v0 + m_edge2;
                
                a = ma.x-mb.x;
                b = ma.y-mb.y;
                c = ma.z-mb.z;

                d = ma.x-mc.x;
                e = ma.y-mc.y;
                f = ma.z-mc.z;
                
                g = ray.get_direction().x;
                h = ray.get_direction().y;
                i = ray.get_direction().z;
                
                j = ma.x-ray.get_origin().x;
                k = ma.y-ray.get_origin().y;
                l = ma.z-ray.get_origin().z;
                
                eimhf = e*i-h*f;
                gfmdi = g*f-d*i;
                dhmeg = d*h-e*g;
                akmjb = a*k-j*b;
                jcmal = j*c-a*l;
                blmkc = b*l-k*c;

                M = a*eimhf+b*gfmdi+c*dhmeg;
                gamma = (i*akmjb+h*jcmal+g*blmkc)/M;
                beta = (j*eimhf+k*gfmdi+l*dhmeg)/M;

                float ua, ub, uc, va, vb, vc;
                ua = texMap[0] . x;
                va = texMap[0] . y;
                ub = texMap[1] . x;
                vb = texMap[1] . y;
                uc = texMap[2] . x;
                vc = texMap[2] . y;
                hit_record.u = ua + beta * (ub-ua) + gamma * (uc-ua);
                hit_record.v = va + beta * (vb-va) + gamma * (vc-va);
            }

            return true;
        }
        return false;
    }

    bool Triangle::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        //M�ller-Trumbore algorithm
        auto pvec = math::cross(ray.get_direction(), m_edge2);
        auto inv_det = 1.0f / math::dot(m_edge1, pvec);

        auto tvec = ray.get_origin() - m_v0;
        auto w1 = math::dot(tvec, pvec) * inv_det;

        if (w1 < 0.0f || w1 > 1.0f)
        {
            return false;
        }

        auto qvec = math::cross(tvec, m_edge1);
        auto w2 = math::dot(ray.get_direction(), qvec) * inv_det;

        if (w2 < 0.0f || (w1 + w2) > 1.0f)
        {
            return false;
        }

        auto distance = math::dot(m_edge2, qvec) * inv_det;
        return distance > 0.0f && distance < max_distance;
    }
}
