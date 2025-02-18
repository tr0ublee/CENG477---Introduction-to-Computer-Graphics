#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include <math.h>

namespace parser
{
    //Notice that all the structures are as simple as possible
    //so that you are not enforced to adopt any style or design.
    struct Vec3f
    {
        inline Vec3f operator*(float a) {
            Vec3f retVal;
            retVal.x = x * a;
            retVal.y = y * a;
            retVal.z = z * a;
            return retVal;
        }

        inline Vec3f operator/(float a) {
            Vec3f retVal;
            retVal.x = x / a;
            retVal.y = y / a;
            retVal.z = z / a;
            return retVal;
        }

        inline Vec3f operator+(const Vec3f& rhs) {
            Vec3f retVal;
            retVal.x = x + rhs.x;
            retVal.y = y + rhs.y;
            retVal.z = z + rhs.z;
            return retVal;
        }

        inline Vec3f operator-(const Vec3f& rhs) {
            Vec3f retVal;
            retVal.x = x - rhs.x;
            retVal.y = y - rhs.y;
            retVal.z = z - rhs.z;
            return retVal;
        }

        Vec3f() {}
        Vec3f(float X, float Y, float Z) {
            x = X;
            y = Y;
            z = Z;
        }

        void normalize() {
            float length = sqrt(x*x + y*y + z*z);
            x /= length;
            y /= length;
            z /= length;
        }
        
        static Vec3f cross(const Vec3f& vec1, const Vec3f& vec2) {
            Vec3f retVal;
            retVal.x = vec1.y * vec2.z - vec1.z * vec2.y;
            retVal.y =  vec1.z * vec2.x - vec1.x * vec2.z;
            retVal.z = vec1.x * vec2.y - vec1.y * vec2.x;
            return retVal;
        }

        float x, y, z;
    };

    struct Vec3i
    {
        inline Vec3i operator*(int a) {
            Vec3i retVal;
            retVal.x = x * a;
            retVal.y = y * a;
            retVal.z = z * a;
            return retVal;
        }
        int x, y, z;
    };

    struct Vec4f
    {
        inline Vec4f operator*(float a) {
            Vec4f retVal;
            retVal.x = x * a;
            retVal.y = y * a;
            retVal.z = z * a;
            retVal.w = w * a;

            return retVal;
        }
        float x, y, z, w;
    };

    struct Camera
    {
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec4f near_plane;
        float near_distance;
        float far_distance;
        int image_width, image_height;
    };

    struct PointLight
    {
        Vec3f position;
        Vec3f intensity;
        bool status;
    };

    struct Material
    {
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        float phong_exponent;
    };

    struct Transformation
    {
        std::string transformation_type;
        int id;
    };

    struct Face
    {
        int v0_id;
        int v1_id;
        int v2_id;
    };

    struct Mesh
    {
        int material_id;
        std::vector<Face> faces;
        std::vector<Transformation> transformations;
        std::string mesh_type;
    };

    struct Scene
    {
        //Data
        Vec3i background_color;
        int culling_enabled;
        int culling_face;
        Camera camera;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Vec3f> vnormal_data; // vertex normal, our own data. This is filled in the main.cpp fillVertexNormals func.
        std::vector<Vec3f> translations;
        std::vector<Vec3f> scalings;
        std::vector<Vec4f> rotations;
        std::vector<Mesh> meshes;

        //Functions
        void loadFromXml(const std::string& filepath);
    };
}

#endif