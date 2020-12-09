#include "scene.h"
#include <iostream>

using namespace std;

namespace fst
{
    void Scene::loadFromParser(const parser::Scene& parser)
    {
        for (auto& camera : parser.cameras)
        {
            cameras.push_back(Camera(
                math::Vector3f(camera.position.x, camera.position.y, camera.position.z),
                math::Vector3f(camera.gaze.x, camera.gaze.y, camera.gaze.z),
                math::Vector3f(camera.up.x, camera.up.y, camera.up.z),
                math::Vector4f(camera.near_plane.x, camera.near_plane.y, camera.near_plane.z, camera.near_plane.w),
                math::Vector2f(camera.image_width, camera.image_height),
                camera.image_name,
                camera.near_distance));
        }

        for (auto& pointlight : parser.point_lights)
        {
            point_lights.push_back(PointLight(
                math::Vector3f(pointlight.position.x, pointlight.position.y, pointlight.position.z),
                math::Vector3f(pointlight.intensity.x, pointlight.intensity.y, pointlight.intensity.z)));
        }

        for (auto& material : parser.materials)
        {
            materials.push_back(Material(
                math::Vector3f(material.ambient.x, material.ambient.y, material.ambient.z),
                math::Vector3f(material.diffuse.x, material.diffuse.y, material.diffuse.z),
                math::Vector3f(material.specular.x, material.specular.y, material.specular.z),
                math::Vector3f(material.mirror.x, material.mirror.y, material.mirror.z),
                material.phong_exponent));
        }

        for (auto &translation : parser.translations)
        {
            translations.push_back(
                Translation(translation.x, translation.y, translation.z));
        }

        for (auto &scaling : parser.scalings)
        {
            scalings.push_back(
                Scaling(scaling.x, scaling.y, scaling.z));
        }

        for (auto &rotation : parser.rotations)
        {
            rotations.push_back(
                Rotation(rotation.angle, rotation.x, rotation.y, rotation.z));
        }

        for (auto& vertex : parser.vertex_data)
        {
            vertex_data.push_back(math::Vector3f(vertex.x, vertex.y, vertex.z));
        }

        for (auto& tex_coord : parser.tex_coord_data)
        {
            tex_coord_data.push_back(math::Vector2f(tex_coord.x, tex_coord.y));
        }

        for (auto& mesh : parser.meshes)
        {
            // transformation parse
            std::vector<struct Transformation> transformations = ParseTransformationString(mesh.transformations);

            std::vector<Triangle> triangles;
            for (auto& face : mesh.faces)
            {   
                // transformation
                math::Vector3f v0 = vertex_data[face.v0_id - 1];
                math::Vector3f v1 = vertex_data[face.v1_id - 1];
                math::Vector3f v2 = vertex_data[face.v2_id - 1];
                math::Vector4f result0(v0.x, v0.y, v0.z, 1);
                math::Vector4f result1(v1.x, v1.y, v1.z, 1);
                math::Vector4f result2(v2.x, v2.y, v2.z, 1);
                for (auto& transformation : transformations) {
                    if (transformation.type == TRANSLATE) {
                        Translation t = translations[transformation.index];
                        Matrix m = t.getTranslationMatrix();
                        result0 = m * result0;
                        result1 = m * result1;
                        result2 = m * result2;
                    }
                    else if (transformation.type == SCALE) { /*
                        Scaling s = scalings[transformation.index];
                        Matrix m = s.getScalingMatrix();
                        result0 = m * result0;
                        result1 = m * result1;
                        result2 = m * result2; */
                    }
                    else { /*
                        Rotation r = rotations[transformation.index];
                        Matrix m = r.getRotationMatrix();

                        Translation t0(v0.x, v0.y, v0.z);
                        Translation t1(v1.x, v1.y, v1.z);
                        Translation t2(v2.x, v2.y, v2.z);

                        result0 = t0.getInverseTranslationMatrix() * m * t0.getTranslationMatrix() * result0;
                        result1 = t1.getInverseTranslationMatrix() * m * t1.getTranslationMatrix() * result1;
                        result2 = t2.getInverseTranslationMatrix() * m * t2.getTranslationMatrix() * result2; */
                    }
                }
                v0 = result0;
                v1 = result1;
                v2 = result2;
                /** Transformation End **/

                triangles.push_back(Triangle(
                    v0,
                    v1 - v0,
                    v2 - v0));
            }
            
            meshes.push_back(Mesh(std::move(triangles), mesh.material_id));
        }

        for (auto& triangle : parser.triangles)
        {
            // transformation parse
            std::vector<struct Transformation> transformations = ParseTransformationString(triangle.transformations);

            // transformation
            math::Vector3f v0 = vertex_data[triangle.indices.v0_id - 1];
            math::Vector3f v1 = vertex_data[triangle.indices.v1_id - 1];
            math::Vector3f v2 = vertex_data[triangle.indices.v2_id - 1];
            math::Vector4f result0(v0.x, v0.y, v0.z, 1);
            math::Vector4f result1(v1.x, v1.y, v1.z, 1);
            math::Vector4f result2(v2.x, v2.y, v2.z, 1);
            for (auto& transformation : transformations) {
                if (transformation.type == TRANSLATE) {
                    Translation t = translations[transformation.index];
                    Matrix m = t.getTranslationMatrix();
                    cout << m << endl;
                    result0 = m * result0;
                    result1 = m * result1;
                    result2 = m * result2;
                }
                else if (transformation.type == SCALE) { /*
                    Scaling s = scalings[transformation.index];
                    Matrix m = s.getScalingMatrix();
                    result0 = m * result0;
                    result1 = m * result1;
                    result2 = m * result2; */
                }
                else { /*
                    Rotation r = rotations[transformation.index];
                    Matrix m = r.getRotationMatrix();

                    Translation t0(v0.x, v0.y, v0.z);
                    Translation t1(v1.x, v1.y, v1.z);
                    Translation t2(v2.x, v2.y, v2.z);

                    result0 = t0.getInverseTranslationMatrix() * m * t0.getTranslationMatrix() * result0;
                    result1 = t1.getInverseTranslationMatrix() * m * t1.getTranslationMatrix() * result1;
                    result2 = t2.getInverseTranslationMatrix() * m * t2.getTranslationMatrix() * result2; */
                }
            }
            v0 = result0;
            v1 = result1;
            v2 = result2;
            /** Transformation End **/

            std::vector<Triangle> triangles;

            triangles.push_back(Triangle(
                v0,
                v1 - v0,
                v2 - v0));

            meshes.push_back(Mesh(std::move(triangles), triangle.material_id));
        }

        for (auto& sphere : parser.spheres)
        {
            spheres.push_back(Sphere(vertex_data[sphere.center_vertex_id - 1],
                sphere.radius, sphere.material_id));
        }

        background_color = math::Vector3f(parser.background_color.x, parser.background_color.y, parser.background_color.z);
        ambient_light = math::Vector3f(parser.ambient_light.x, parser.ambient_light.y, parser.ambient_light.z);
        shadow_ray_epsilon = parser.shadow_ray_epsilon;
        max_recursion_depth = parser.max_recursion_depth;
    }

    bool Scene::intersect(const Ray& ray, HitRecord& hit_record, float max_distance) const
    {
        HitRecord temp;
        float min_distance = max_distance;
        for (auto& sphere : spheres)
        {
            if (sphere.intersect(ray, temp, min_distance))
            {
                min_distance = temp.distance;
                hit_record = temp;
            }
        }

        for (auto& mesh : meshes)
        {
            if (mesh.intersect(ray, temp, min_distance))
            {
                min_distance = temp.distance;
                hit_record = temp;
            }
        }

        return min_distance != max_distance;
    }

    bool Scene::intersectShadowRay(const Ray& ray, float max_distance) const
    {
        for (auto& sphere : spheres)
        {
            if (sphere.intersectShadowRay(ray, max_distance))
            {
                return true;
            }
        }


        for (auto& mesh : meshes)
        {
            if (mesh.intersectShadowRay(ray, max_distance))
            {
		return true;
            }
        }

	return false;
    }

    std::vector<struct Transformation> Scene::ParseTransformationString (std::string transformationString) {
        std::vector<struct Transformation> transformations;
        for (int i=0; i < transformationString.size(); i++) {
            if (transformationString[i] == 't') {
                Transformation t = {TRANSLATE, transformationString[i+1] - '1'};
                transformations.push_back(t);
            }
            else if (transformationString[i] == 's') {
                Transformation t = {SCALE, transformationString[i+1] - '1'};
                transformations.push_back(t);
            }
            else if (transformationString[i] == 'r') {
                Transformation t = {ROTATE, transformationString[i+1] - '1'};
                transformations.push_back(t);
            }
        }
        return transformations;
    }
}
