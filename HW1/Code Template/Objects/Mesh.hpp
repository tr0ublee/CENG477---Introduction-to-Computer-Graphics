#include "Material.hpp"
#include <vector>

class Mesh{
    public:

        Material* material;
        std::vector<std::vector<double>> Faces;

        Mesh(const Material &a, std::vector<std::vector<double>> &faces) {
            material = new Material(a);
            for(int i = 0; faces.size();i++) {
                for(int j = 0; j < faces[i].size(); j++) {
                    Faces[i].push_back(faces[i][j]);
                }
            }
        }
        ~Mesh() {
            delete material;
        }
};