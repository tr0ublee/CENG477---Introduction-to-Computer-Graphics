#include "Material.hpp"


Material::Material(Vec3 &ambient, Vec3 &diff, Vec3 &spec, Vec3 &mirr, int phong) {
    ambientReflectance = new Vec3(ambient);
    diffuseReflectance = new Vec3(diff);
    specularReflectance = new Vec3(spec);
    mirrorReflectance = new Vec3(mirr);
    isMirror = !((mirrorReflectance -> x == 0.0f) && (mirrorReflectance -> y == 0.0f) && (mirrorReflectance -> z == 0.0f));
    phongExponent = phong;
}
Material::Material(const Material &a) {
    ambientReflectance = new Vec3(*(a.ambientReflectance));
    diffuseReflectance = new Vec3(*(a.diffuseReflectance));
    specularReflectance = new Vec3(*(a.specularReflectance));
    mirrorReflectance = new Vec3(*(a.mirrorReflectance));
    isMirror = !((mirrorReflectance -> x == 0.0f) && (mirrorReflectance -> y == 0.0f) && (mirrorReflectance -> z == 0.0f));
    phongExponent = a.phongExponent;
}

Material::~Material() {
    delete ambientReflectance;
    delete diffuseReflectance;
    delete specularReflectance;
    delete mirrorReflectance;
}
