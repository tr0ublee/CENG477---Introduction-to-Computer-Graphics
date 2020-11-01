#include "Camera.hpp"

Camera::Camera(Vec3 &posX, Vec3 &gazeX, Vec3 &vX, Vec3 &uX, float l, float r, float b, float t, float d, int width, int height, const std::string &name) {
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

Camera::Camera(parser::Camera cameraStruct) {
    pos = new Vec3(cameraStruct.position.x, cameraStruct.position.y, cameraStruct.position.z);
    gaze = new Vec3(cameraStruct.gaze.x, cameraStruct.gaze.y, cameraStruct.gaze.z);
    v = new Vec3(cameraStruct.up.x, cameraStruct.up.y, cameraStruct.up.z);
    v -> normalize();
    gaze -> normalize();
    Vec3 w((*gaze) * -1);
    u = new Vec3(v -> cross(*v,w));
    u -> normalize();
    l = cameraStruct.near_plane.x;
    r = cameraStruct.near_plane.y;
    b = cameraStruct.near_plane.z;
    t = cameraStruct.near_plane.w;
    d = cameraStruct.near_distance;
    m = new Vec3((*pos) + (*gaze) * d);
    q = new Vec3((*m) + (*u) * l + (*v) * t);
    imageHeight = cameraStruct.image_height;
    imageWidth = cameraStruct.image_width;
    imageName = cameraStruct.image_name;
    pixelW = (r-l) / (float) imageWidth;
    pixelH = (t-b) / (float) imageHeight;
    halfPixelW = pixelW * 0.5;
    halfPixelH = pixelH * 0.5;

  
}

Camera::~Camera() {
    delete pos;
    delete gaze;
    delete v;
    delete m;
    delete q;
    delete u;
}