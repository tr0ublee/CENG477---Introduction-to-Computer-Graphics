#include "Vec3.hpp"

class Ray{

    public:

        Vec3* a;
        Vec3* b;

        Ray(Vec3 &a, Vec3 &b) {
            this -> a = new Vec3(a);
            this -> b = new Vec3(b);
        }
        ~Ray() {
            delete a;
            delete b;
        }


};