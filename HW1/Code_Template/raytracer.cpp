#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "./Objects/Triangle.hpp"
#include "./Objects/Sphere.hpp"
#include "./Objects/Ray.hpp"
#include "./Objects/Camera.hpp"
#include "./Objects/Mesh.hpp"
#include "./Objects/Scene.hpp"


typedef unsigned char RGB[3];
typedef unsigned char* Image;
using namespace std;

void initScene(const char* path, Scene** scene) {
   parser::Scene* tmpScene = new parser::Scene();
   tmpScene->loadFromXml(path);
   *scene = new Scene(*tmpScene);
   delete tmpScene;
   tmpScene = nullptr;
}

void initImage(Image image, Scene* scene, int width, int height) {
    char r = (char) scene -> background -> r;
    char g = (char) scene -> background -> g; 
    char b = (char) scene -> background -> b;
    size_t i = 0;
    cout << *image << endl;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image[i++] = r; // r
            image[i++] = g; // g
            image[i++] = b; // b
        }
    }
}

int main(int argc, char* argv[]){
    
    if (argc < 2) {
        std::cerr << "No XML provided" << endl;
        return -1;
    }
    Scene* scene;
    initScene(argv[1], &scene);
    int numOfCams = scene -> numOfCameras;
    for (size_t i = 0; i < numOfCams; i++) {
        Camera* currentCam = scene -> cameras[i];
        int imageWidth = currentCam -> imageWidth;
        int imageHeight = currentCam -> imageHeight;
        Image image = (Image) std::malloc(sizeof(unsigned char)*(imageWidth*imageHeight*3)); // [RGB | RGB | RGB...]
        initImage(image, scene, imageWidth, imageHeight);
        write_ppm("test.ppm", image, imageWidth, imageHeight);
        break;



    }

}

//backup
/*
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    int width = 640, height = 480;
    int columnWidth = width / 8;

    unsigned char* image = new unsigned char [width * height * 3];

    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int colIdx = x / columnWidth;
            image[i++] = BAR_COLOR[colIdx][0];
            image[i++] = BAR_COLOR[colIdx][1];
            image[i++] = BAR_COLOR[colIdx][2];
        }
    }

    write_ppm("test.ppm", image, width, height);

*/


