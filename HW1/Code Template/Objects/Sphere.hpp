#include "Vec3.hpp"
#include "Color.hpp"

class Sphere {
    
    public:

        Vec3* center;
        double r;
        Color* color;

        Sphere(Vec3 &a, double r, Color &c) {
            center = new Vec3(a);
            this -> r = r;
            color = new Color(c);
        }

        ~Sphere() {
            delete center;
            delete color;
        }
};