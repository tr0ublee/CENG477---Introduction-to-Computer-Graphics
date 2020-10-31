#include "PointLight.hpp"

PointLight::PointLight(Vec3 &position, Vec3 &intensity) {
    pos = new Vec3(position);
    this -> intensity = new Vec3(intensity);
}

PointLight::~PointLight() {
    delete pos;
    delete intensity;
}