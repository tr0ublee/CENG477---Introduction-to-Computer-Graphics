#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Vec3.hpp"
#include "../parser.h"

class Camera {

    public:
        Vec3* pos;      // position
        Vec3* gaze;     // -w
        Vec3* v;        // up vector
        Vec3* u;        // v x w
        double l;       // left
        double r;       // right
        double b;       // bottom
        double t;       // top
        double d;       // distance
        long imageWidth;
        long imageHeight;
        std::string imageName;

        Camera(Vec3 &posX, Vec3 &gazeX, Vec3 &vX, Vec3 &uX, double l, double r, 
                double b, double t, double d, long width, long height, const std::string &name);
        Camera(parser::Camera cameraStruct);
        ~Camera();

};
#endif