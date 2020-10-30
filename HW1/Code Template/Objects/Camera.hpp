#include "Vec3.hpp"

class Camera {

    public:
        Vec3* pos;
        Vec3* gaze;
        Vec3* v;
        Vec3* u;
        double l;
        double r;
        double b;
        double t;
        double d;
        long imageWidth;
        long imageHeight;
        std::string imageName;

        Camera(Vec3 &posX, Vec3 &gazeX, Vec3 &vX, Vec3 &uX, double l, double r, 
                double b, double t, double d, long width, long height, const std::string &name);
        ~Camera();

};