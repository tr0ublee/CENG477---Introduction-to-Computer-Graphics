#include "Camera.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "PointLight.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include <vector>


class Scene{

    std::vector<Camera*> cameras;
    std::vector<PointLight*> lights;
    Color* backGround;
    double shadowRayEpsilon;
    Vec3* ambientLight;
    std::vector<Material*> materials;
    std::vector<std::vector<double>> vertexData;
    std::vector<Mesh*> meshes;
    std::vector<Triangle*> triangles;
    std::vector<Sphere*> spheres;

    Scene();

    ~Scene();



};