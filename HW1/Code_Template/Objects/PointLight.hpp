#include "Vec3.hpp"

class PointLight{

    public:

        Vec3* pos;
        Vec3* intensity;

        PointLight(Vec3 &position, Vec3 &intensity);

        ~PointLight();

};