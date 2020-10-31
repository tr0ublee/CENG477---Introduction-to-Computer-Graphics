#ifndef SCENE_HPP
#define SCENE_HPP

#include "Camera.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "PointLight.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include <vector>
#include <iostream>
#include "../tinyxml2.h"
#include "../parser.h"

class Scene{

    public:
        Color* background;
        float shadowRayEpsilon;
        int maxRecursionDepth;
        Vec3* ambientLight;
        size_t numOfCameras;
        size_t numOfLights;
        size_t numOfMaterials;
        size_t numOfVertices;
        size_t numOfMeshes;
        size_t numOfTriangles;
        size_t numOfSpheres;
        std::vector<Camera*> cameras;
        std::vector<PointLight*> lights;
        std::vector<Material*> materials;
        std::vector<Vec3*> vertexData;
        std::vector<Mesh*> meshes;
        std::vector<Triangle*> triangles;
        std::vector<Sphere*> spheres;
        Scene();
        Scene(parser::Scene sceneStruct);
        ~Scene();
};

#endif