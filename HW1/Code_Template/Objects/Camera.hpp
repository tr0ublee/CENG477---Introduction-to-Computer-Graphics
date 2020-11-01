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
        Vec3* m;
        Vec3* q;
        float l;       // left
        float r;       // right
        float b;       // bottom
        float t;       // top
        float d;       // distance
        float pixelW;
        float pixelH;
        float halfPixelW;
        float halfPixelH;
        int imageWidth;
        int imageHeight;
        std::string imageName;

        Camera(Vec3 &posX, Vec3 &gazeX, Vec3 &vX, Vec3 &uX, float l, float r, 
                float b, float t, float d, int width, int height, const std::string &name);
        Camera(parser::Camera cameraStruct);
        ~Camera();

};
#endif