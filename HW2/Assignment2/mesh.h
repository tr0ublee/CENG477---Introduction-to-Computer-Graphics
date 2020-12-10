#pragma once

#include "triangle.h"
#include "texture.h"
#include <vector>

namespace fst
{
    class Mesh : public Triangular
    {
    public:
        int texture_id;
        Mesh(const std::vector<Triangle>& triangles, int material_id, int texture_id);
        Mesh(std::vector<Triangle>&& triangles, int material_id, int texture_id);

        bool intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const override;
        bool intersectShadowRay(const Ray& ray, float max_distance) const override;

    private:
	std::vector<Triangle> m_triangles;
        int m_material_id;
    };
}
