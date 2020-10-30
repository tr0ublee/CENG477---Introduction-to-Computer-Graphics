#include "Vec3.hpp"

class Camera {

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

    Camera(Vec3 &posX, Vec3 &gazeX, Vec3 &vX, Vec3 &uX, double l, double r, double b, double t, double d, long width, long height, const std::string &name) {
        pos = new Vec3(posX);
        gaze = new Vec3(gazeX);
        v = new Vec3(vX);
        u = new Vec3(uX);
        this -> l = l;
        this -> r = r;
        this -> b = b;
        this -> t = t;
        this -> d = d;
        imageHeight = height;
        imageWidth = width;
        imageName = name;
    }

    ~Camera() {
        delete pos;
        delete gaze;
        delete v;
        delete u;
    }

};