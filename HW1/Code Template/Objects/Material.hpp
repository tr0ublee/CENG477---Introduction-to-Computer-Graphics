#include "Vec3.hpp"

class Material{ 

    public:
        Vec3* ambientReflectance;
        Vec3* diffuseReflectance;
        Vec3* specularReflectance;
        Vec3* mirrorReflectance;
        int phongExponent;

        Material(Vec3 &a, Vec3 &b, Vec3 &c, Vec3 &d, int phong) {
            ambientReflectance = new Vec3(a);
            diffuseReflectance = new Vec3(b);
            specularReflectance = new Vec3(c);
            mirrorReflectance = new Vec3(d);
            phongExponent = phong;
        }
        Material(const Material &a) {
            ambientReflectance = new Vec3(*(a.ambientReflectance));
            diffuseReflectance = new Vec3(*(a.diffuseReflectance));
            specularReflectance = new Vec3(*(a.specularReflectance));
            mirrorReflectance = new Vec3(*(a.mirrorReflectance));
            phongExponent = a.phongExponent;
        }

        ~Material() {
            delete ambientReflectance;
            delete diffuseReflectance;
            delete specularReflectance;
            delete mirrorReflectance;
        }
};