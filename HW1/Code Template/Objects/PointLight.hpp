#include "Vec3.hpp"

class PointLight{

    Vec3* pos;
    Vec3* intensity;

    PointLight(Vec3 &position, Vec3 &intensity) {
        pos = new Vec3(position);
        this -> intensity = new Vec3(intensity);
    }

    ~PointLight() {
        delete pos;
        delete intensity;
    }

};