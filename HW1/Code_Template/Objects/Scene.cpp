#include "Scene.hpp"
using namespace tinyxml2;
using namespace std;

Scene::Scene(){

}

Scene::Scene(parser::Scene &sceneStruct) {
    background = new Color(sceneStruct.background_color.x, sceneStruct.background_color.y, sceneStruct.background_color.z);
    shadowRayEpsilon = sceneStruct.shadow_ray_epsilon;
    maxRecursionDepth = sceneStruct.max_recursion_depth;
    ambientLight = new Vec3(sceneStruct.ambient_light.x, sceneStruct.ambient_light.y, sceneStruct.ambient_light.z);

    numOfCameras = sceneStruct.cameras.size();
    for (size_t i = 0; i < numOfCameras; i++) {
        Camera* pushed = new Camera(sceneStruct.cameras[i]);
        cameras.push_back(pushed);
    }

    numOfLights = sceneStruct.point_lights.size();
    for (size_t i = 0; i<numOfLights; i++) {
        parser::PointLight current = sceneStruct.point_lights[i];
        Vec3 a = Vec3(current.position.x, current.position.y, current.position.z);
        Vec3 b = Vec3(current.intensity.x, current.intensity.y, current.intensity.z);
        PointLight* pushed = new PointLight(a,b);
        lights.push_back(pushed);
    }

    numOfMaterials = sceneStruct.materials.size();
    for (size_t i = 0; i<numOfMaterials; i++) {
        parser::Material currentMaterial = sceneStruct.materials[i];
        Vec3 ambient = Vec3(currentMaterial.ambient.x, currentMaterial.ambient.y, currentMaterial.ambient.z);
        Vec3 diffuse = Vec3(currentMaterial.diffuse.x, currentMaterial.diffuse.y, currentMaterial.diffuse.z);
        Vec3 specular = Vec3(currentMaterial.specular.x, currentMaterial.specular.y, currentMaterial.specular.z);
        Vec3 mirror = Vec3(currentMaterial.mirror.x, currentMaterial.mirror.y, currentMaterial.mirror.z);
        Material* pushed = new Material(ambient, diffuse, specular, mirror, currentMaterial.phong_exponent);
        materials.push_back(pushed);
    }

    numOfVertices = sceneStruct.vertex_data.size();
    for (size_t i = 0; i<numOfVertices; i++) {
        parser::Vec3f current = sceneStruct.vertex_data[i];
        Vec3* pushed = new Vec3(current.x, current.y, current.z);
        vertexData.push_back(pushed); 
    }

    numOfMeshes = sceneStruct.meshes.size();
    for (size_t i = 0; i<numOfMeshes; i++) {
        parser::Mesh current = sceneStruct.meshes[i];
        size_t faceSize = current.faces.size();
        std::vector<Face> faceVec;

        for (size_t j = 0; j < faceSize; j++){
            parser::Face currentFace = current.faces[j];
            int v0_index = currentFace.v0_id-1;
            int v1_index = currentFace.v1_id-1;
            int v2_index = currentFace.v2_id-1;
            Face pushed = Face(vertexData[v0_index], vertexData[v1_index], vertexData[v2_index]);
            faceVec.push_back(pushed);
        }
        Mesh* pushed = new Mesh(materials[current.material_id-1], faceVec, faceSize);
        meshes.push_back(pushed);
    }

    numOfSpheres = sceneStruct.spheres.size();
    for (size_t i = 0; i<numOfSpheres; i++) {
        parser::Sphere current = sceneStruct.spheres[i];
        int materialIndex = current.material_id-1;
        int vertexIndex = current.center_vertex_id-1;
        double radius = current.radius; 
        Sphere* pushed = new Sphere(materials[materialIndex], vertexData[vertexIndex], radius);
        spheres.push_back(pushed);
    }

    numOfTriangles = sceneStruct.triangles.size();
    for (size_t i = 0; i < numOfTriangles; i++) {
        parser::Triangle current = sceneStruct.triangles[i];
        int materialIndex = current.material_id-1;
        parser::Face currFace = current.indices;
        int v0_index = currFace.v0_id-1;
        int v1_index = currFace.v1_id-1;
        int v2_index = currFace.v2_id-1;
        Face face = Face(vertexData[v0_index], vertexData[v1_index], vertexData[v2_index]);        
        Triangle* pushed = new Triangle(materials[materialIndex], face);
        triangles.push_back(pushed);
    }
}

Scene::~Scene(){
    delete background;
    delete ambientLight;

    for (size_t i = 0; i < numOfCameras; i++) {
        delete cameras[i];
    }

    for (size_t i = 0; i<numOfLights; i++) {
        delete lights[i];
    }

    for (size_t i = 0; i<numOfMaterials; i++) {
        delete materials[i];
    }

    for (size_t i = 0; i<numOfVertices; i++) {
        delete vertexData[i];
    }

    for (size_t i = 0; i<numOfMeshes; i++) {
        delete meshes[i];
    }

    for (size_t i = 0; i<numOfSpheres; i++) {
        delete spheres[i];
    }

    for (size_t i = 0; i < numOfTriangles; i++) {
        delete triangles[i];
    }
}