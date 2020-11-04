#pragma GCC optimization ("unroll-loops")
#pragma GCC target("avx,avx2,fma")
#include <iostream>
#include <thread>
#include <mutex>
#include "parser.h"
#include "ppm.h"
#include "./Objects/Triangle.hpp"
#include "./Objects/Sphere.hpp"
#include "./Objects/Ray.hpp"
#include "./Objects/Camera.hpp"
#include "./Objects/Mesh.hpp"
#include "./Objects/Scene.hpp"
#include "stdio.h"
#include <limits>
#include <chrono>

#define INF std::numeric_limits<float>::max();
typedef unsigned char RGB[3];
typedef unsigned char* Image;
using namespace std;

typedef enum closestObject {
    SPHERE,
    TRIANGLE,
    MESH,
    NONE
} ClosestObject;

int operatingRow = 0;
std::mutex rowMutex;

void printVec(Vec3 a) {
	printf("X:%lf Y:%lf Z:%lf \n", a.x, a.y, a.z);
}
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

int incrementAndQueryRow() {
    std::lock_guard<std::mutex> lock(rowMutex);
    return ++operatingRow;

}

void shade (Scene* scene, Image image, Camera* currentCam, int imageWidth, int imageHeight) {

    size_t y = incrementAndQueryRow();
    while (y < imageHeight){
        size_t colorIndex = y*imageWidth*3;
        for (size_t x = 0; x < imageWidth; x++) {
            Ray* ray = new Ray(x, y, currentCam);
            Sphere* closestSphere = nullptr;
            Triangle* closestTriangle = nullptr;
            Mesh* closestMesh = nullptr;
            Face* closestMeshFace = nullptr;
            ClosestObject closestObject =  NONE;
            float tMin = INF;
            size_t numOfLights = scene -> numOfLights;
            size_t numOfSpheres = scene -> numOfSpheres;
            for (size_t sphereIndex = 0; sphereIndex < numOfSpheres; sphereIndex++) {
                Sphere* currentSphere = scene -> spheres[sphereIndex];
                float t = currentSphere -> intersectRay(ray);
                if (t > 1.0 || FLOAT_EQ(t,1.0f)) {
                    // t >= 1.0
                    if (tMin > t) {
                        // t < tMin
                        tMin = t;
                        closestSphere = currentSphere;
                        closestObject = SPHERE;
                    }
                }
            }
            size_t numOfTriangles = scene -> numOfTriangles;
            for (size_t triangleIndex = 0; triangleIndex < numOfTriangles; triangleIndex++) {
                Triangle* currentTriangle = scene -> triangles[triangleIndex];
                float t = currentTriangle -> indices -> intersectRay(ray);
                if (t > 1.0f || FLOAT_EQ(t,1.0f)) {
                    // t >= 1.0
                    if (tMin > t) {
                        // t < tMin
                        tMin = t;
                        closestTriangle = currentTriangle;
                        closestObject = TRIANGLE;
                    }
                }
            }
            size_t numOfMeshes = scene -> numOfMeshes;
            for (size_t meshIndex = 0; meshIndex < numOfMeshes; meshIndex++) {
                Mesh* currentMesh = scene -> meshes[meshIndex];
                size_t numOfFaces = currentMesh -> numOfFaces;
                for (size_t faceIndex = 0; faceIndex < numOfFaces; faceIndex++) {
                    Face* currentFace = currentMesh -> faces[faceIndex];;
                    float t = currentFace -> intersectRay(ray);
                    if (t > 1.0f || FLOAT_EQ(t, 1.0f)) {
                        // t >= 1.0
                        if (t < tMin) {
                            // t < tMin
                            tMin = t;
                            closestMeshFace = currentFace;
                            closestMesh = currentMesh;
                            closestObject = MESH;
                        }
                    }
                }
            }

            if (closestObject == SPHERE && closestSphere != nullptr) { // shading
                // ambient
                float kr = scene -> ambientLight -> x;
                float kg = scene -> ambientLight -> y;
                float kb = scene -> ambientLight -> z;
                float ir = closestSphere -> material -> ambientReflectance -> x;
                float ig = closestSphere -> material -> ambientReflectance -> y;
                float ib = closestSphere -> material -> ambientReflectance -> z;
                // rgb values
                double red = kr * ir;
                double green = kg * ig;
                double blue = kb * ib;
                // shading for each light
                for (size_t lightIndex = 0; lightIndex < numOfLights; lightIndex++) {
                    // diffuse
                    PointLight* currentLight = scene -> lights[lightIndex];
                    Vec3 intersectionPoint = Vec3(*(ray -> e) + *(ray -> d) * tMin); 
                    Vec3 wi = *currentLight -> pos - intersectionPoint;
                    Vec3 normal = intersectionPoint - *(closestSphere ->center);
                    wi.normalize();
                    normal.normalize();
                    float costheta = std::max(0.0f, wi.dot(normal));
                    float distanceSquare = intersectionPoint.distanceSquare(*(currentLight -> pos)); 
                    float rCoef = closestSphere -> material -> diffuseReflectance -> x;
                    float gCoef = closestSphere -> material -> diffuseReflectance -> y;
                    float bCoef = closestSphere -> material -> diffuseReflectance -> z;
                    float rIntensity = currentLight -> intensity -> x;
                    float gIntensity = currentLight -> intensity -> y;
                    float bIntensity = currentLight -> intensity -> z; 
                    float Er = rIntensity / distanceSquare;
                    float Eg = gIntensity / distanceSquare;
                    float Eb = bIntensity / distanceSquare;

                    red += rCoef * costheta* Er;
                    green += gCoef * costheta* Eg;
                    blue += bCoef * costheta* Eb;

                    // specular
                    Vec3 wo = *(currentCam -> pos) - intersectionPoint;
                    wo.normalize();
                    Vec3 wio = wi + wo;
                    Vec3 h = wio / wio.getLength();
                    h.normalize();
                    float cosalpha = std::max(0.0f, normal.dot(h));
                    cosalpha = std::pow(cosalpha, closestSphere -> material -> phongExponent);
                    red += (closestSphere -> material -> specularReflectance -> x) * cosalpha * Er;
                    green += (closestSphere -> material -> specularReflectance -> y) * cosalpha * Eg;
                    blue += (closestSphere -> material -> specularReflectance -> z) * cosalpha * Eb;
                }

                image[colorIndex] = red > 255 ? 255 : round(red);
                image[colorIndex+1] = green > 255 ? 255 : round(green);
                image[colorIndex+2] = blue > 255 ? 255 : round(blue);
            } else if (closestObject == TRIANGLE && closestTriangle != nullptr) {
                // ambient
                Face* face = closestTriangle -> indices;
                float kr = scene -> ambientLight -> x;
                float kg = scene -> ambientLight -> y;
                float kb = scene -> ambientLight -> z;
                float ir = closestTriangle -> material -> ambientReflectance -> x;
                float ig = closestTriangle -> material -> ambientReflectance -> y;
                float ib = closestTriangle -> material -> ambientReflectance -> z;
                // rgb values
                double red = kr * ir;
                double green = kg * ig;
                double blue = kb * ib;
                // shading for each light
                for (size_t lightIndex = 0; lightIndex < numOfLights; lightIndex++) {
                    // diffuse
                    PointLight* currentLight = scene -> lights[lightIndex];
                    Vec3 intersectionPoint = Vec3(*(ray -> e) + *(ray -> d) * tMin); 
                    Vec3 wi = *currentLight -> pos - intersectionPoint;
                    Vec3 normal = *(face  -> normal) ;
                    wi.normalize();
                    float costheta = std::max(0.0f, wi.dot(normal));
                    float distanceSquare = intersectionPoint.distanceSquare(*(currentLight -> pos)); 
                    float rCoef = closestTriangle -> material -> diffuseReflectance -> x;
                    float gCoef = closestTriangle -> material -> diffuseReflectance -> y;
                    float bCoef = closestTriangle -> material -> diffuseReflectance -> z;
                    float rIntensity = currentLight -> intensity -> x;
                    float gIntensity = currentLight -> intensity -> y;
                    float bIntensity = currentLight -> intensity -> z; 
                    float Er = rIntensity / distanceSquare;
                    float Eg = gIntensity / distanceSquare;
                    float Eb = bIntensity / distanceSquare;

                    red += rCoef * costheta* Er;
                    green += gCoef * costheta* Eg;
                    blue += bCoef * costheta* Eb;

                    // specular
                    Vec3 wo = *(currentCam -> pos) - intersectionPoint;
                    wo.normalize();
                    Vec3 wio = wi + wo;
                    Vec3 h = wio / wio.getLength();
                    h.normalize();
                    float cosalpha = std::max(0.0f, normal.dot(h));
                    cosalpha = std::pow(cosalpha, closestTriangle -> material -> phongExponent);
                    red += (closestTriangle -> material -> specularReflectance -> x) * cosalpha * Er;
                    green += (closestTriangle -> material -> specularReflectance -> y) * cosalpha * Eg;
                    blue += (closestTriangle -> material -> specularReflectance -> z) * cosalpha * Eb;
                }

                image[colorIndex] = red > 255 ? 255 : round(red);
                image[colorIndex+1] = green > 255 ? 255 : round(green);
                image[colorIndex+2] = blue > 255 ? 255 : round(blue);

            } else if (closestObject == MESH && closestMesh != nullptr && closestMeshFace != nullptr) {
                // ambient
                float kr = scene -> ambientLight -> x;
                float kg = scene -> ambientLight -> y;
                float kb = scene -> ambientLight -> z;
                float ir = closestMesh -> material -> ambientReflectance -> x;
                float ig = closestMesh -> material -> ambientReflectance -> y;
                float ib = closestMesh -> material -> ambientReflectance -> z;
                // rgb values
                double red = kr * ir;
                double green = kg * ig;
                double blue = kb * ib;
                // shading for each light
                for (size_t lightIndex = 0; lightIndex < numOfLights; lightIndex++) {
                    // diffuse
                    PointLight* currentLight = scene -> lights[lightIndex];
                    Vec3 intersectionPoint = Vec3(*(ray -> e) + *(ray -> d) * tMin); 
                    Vec3 wi = *currentLight -> pos - intersectionPoint;
                    Vec3 normal = *(closestMeshFace -> normal) ;
                    wi.normalize();
                    float costheta = std::max(0.0f, wi.dot(normal));
                    float distanceSquare = intersectionPoint.distanceSquare(*(currentLight -> pos)); 
                    float rCoef = closestMesh -> material -> diffuseReflectance -> x;
                    float gCoef = closestMesh -> material -> diffuseReflectance -> y;
                    float bCoef = closestMesh -> material -> diffuseReflectance -> z;
                    float rIntensity = currentLight -> intensity -> x;
                    float gIntensity = currentLight -> intensity -> y;
                    float bIntensity = currentLight -> intensity -> z; 
                    float Er = rIntensity / distanceSquare;
                    float Eg = gIntensity / distanceSquare;
                    float Eb = bIntensity / distanceSquare;

                    red += rCoef * costheta* Er;
                    green += gCoef * costheta* Eg;
                    blue += bCoef * costheta* Eb;

                    // specular
                    Vec3 wo = *(currentCam -> pos) - intersectionPoint;
                    wo.normalize();
                    Vec3 wio = wi + wo;
                    Vec3 h = wio / wio.getLength();
                    h.normalize();
                    float cosalpha = std::max(0.0f, normal.dot(h));
                    cosalpha = std::pow(cosalpha, closestMesh -> material -> phongExponent);
                    red += (closestMesh-> material -> specularReflectance -> x) * cosalpha * Er;
                    green += (closestMesh -> material -> specularReflectance -> y) * cosalpha * Eg;
                    blue += (closestMesh -> material -> specularReflectance -> z) * cosalpha * Eb;
                }

                image[colorIndex] = red > 255 ? 255 : round(red);
                image[colorIndex+1] = green > 255 ? 255 : round(green);
                image[colorIndex+2] = blue > 255 ? 255 : round(blue);

            }

            colorIndex += 3;
            delete ray;
        }

        y = incrementAndQueryRow();

    }
}
int main(int argc, char* argv[]){
    
    if (argc < 2) {
        std::cerr << "No XML provided" << endl;
        return -1;
    }

    /** Time **/
    std::chrono::time_point<std::chrono::system_clock> start, end; 
    start = std::chrono::system_clock::now();
    /** Time **/

    Scene* scene;
    initScene(argv[1], &scene);
    int numOfCams = scene -> numOfCameras;


    for (size_t i = 0; i < numOfCams; i++) {
        uint cpuCoreNumber = thread::hardware_concurrency();
        std::vector<std::thread*> threads(cpuCoreNumber);
        Camera* currentCam = scene -> cameras[i];
        int imageWidth = currentCam -> imageWidth;
        int imageHeight = currentCam -> imageHeight;
        Image image = (Image) std::malloc(sizeof(unsigned char)*(imageWidth*imageHeight*3)); // [RGB | RGB | RGB...]
        initImage(image, scene, imageWidth, imageHeight);
        if (cpuCoreNumber) {

            for (int threadIndex = 0; threadIndex < cpuCoreNumber; threadIndex++) {
                threads[threadIndex] = new std::thread(shade, scene, image, currentCam, imageWidth, imageHeight);
            }

            for (int threadIndex = 0; threadIndex < cpuCoreNumber; threadIndex++) {
                if(threads[threadIndex]){
                    threads[threadIndex] -> join();
                    delete threads[threadIndex];
                    threads[threadIndex] = nullptr;
                }
            }

        } else {
            // no threading;
            shade(scene, image, currentCam, imageWidth, imageHeight);
        }
        // for (size_t y = 0; y< imageHeight; y++) {
            
                
        // }
        
        write_ppm("test.ppm", image, imageWidth, imageHeight);
        break;
    }

    /** Time **/
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n"; 
    /** Time **/

    delete scene;
    return 0;
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


