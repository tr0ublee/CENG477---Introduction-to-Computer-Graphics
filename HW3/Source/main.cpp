#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#define TRANSLATE "Translation"
#define SCALE "Scaling"
#define ROTATE "Rotation"

using namespace std;

//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void init() {
    glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
    if (scene.culling_enabled) { 
        glEnable(GL_CULL_FACE);  
        if (scene.culling_face) {
            // = 1, frontface
            glCullFace(GL_FRONT);  
        } else {
            glCullFace(GL_BACK);    
        }
    }
}

void initCamera() {
    parser::Camera& cam = scene.camera;
    const parser::Vec3f center = cam.position + cam.gaze * cam.near_distance; 
    glViewport(0, 0, cam.image_width, cam.image_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(  cam.position.x, cam.position.y, cam.position.z, 
                center.x, center.y, center.z, 
                cam.up.x, cam.up.y, cam.up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // easier than gluPerspective 
    glFrustum(cam.near_plane.x, cam.near_plane.y, cam.near_plane.z, cam.near_plane.w, cam.near_distance, cam.far_distance);
    glMatrixMode(GL_MODELVIEW); // do this for transformations

}

void setColor(int materialId) {
    const parser::Material& material = scene.materials[materialId-1];
    GLfloat ambColor[4] = {material.ambient.x, material.ambient.y, material.ambient.z, 1.0};
    GLfloat diffColor[4] = {material.diffuse.x, material.diffuse.y, material.diffuse.z, 1.0};
    GLfloat specColor[4] = {material.specular.x, material.specular.y, material.specular.z, 1.0};
    GLfloat specExp[1] = {material.phong_exponent};

    glMaterialfv(GL_FRONT , GL_AMBIENT , ambColor);
    glMaterialfv(GL_FRONT , GL_DIFFUSE , diffColor);
    glMaterialfv(GL_FRONT , GL_SPECULAR , specColor);
    glMaterialfv(GL_FRONT , GL_SHININESS , specExp);
    
}

void transform(const parser::Transformation& ts) {
    if (ts.transformation_type == TRANSLATE) {
        const parser::Vec3f& translation = scene.translations[ts.id - 1];
        glTranslatef(translation.x, translation.y, translation.z);

    }else if (ts.transformation_type == ROTATE) {
        const parser::Vec4f& rotation = scene.rotations[ts.id - 1];
        glRotatef(rotation.x,rotation.y, rotation.z, rotation.w);
    }else {
        const parser::Vec3f& scaling = scene.scalings[ts.id - 1];
        glScalef(scaling.x, scaling.y, scaling.z);
    }
}

void drawObject() {
	glClearColor(scene.background_color.x, scene.background_color.y, scene.background_color.z, 1);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	static int vertexPosDataSizeInBytes;
    static int vertexNormalPosDataSizeInBytes;
    static int indexDataSizeInBytes;
	static int indiceCount = 0;
    static bool firstTime = true;

	if (firstTime)
	{
		firstTime = false;

		glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        int vertexCount = scene.vertex_data.size();
        
        for (parser::Mesh& mesh : scene.meshes) {
            for (parser::Face& face : mesh.faces) {
                indiceCount += 3;
            }
        }

		GLuint indices[indiceCount]; // faces;
		GLfloat vertexPos[vertexCount * 3]; // vertex_data;
        GLfloat vertexNormalPos[vertexCount * 3]; // vnormal_data;

        int indiceIndex = 0;
        for (parser::Mesh& mesh : scene.meshes) {
            for (parser::Face& face : mesh.faces) {
                indices[indiceIndex]     = face.v0_id - 1;
                indices[indiceIndex + 1] = face.v1_id - 1;
                indices[indiceIndex + 2] = face.v2_id - 1;
                indiceIndex += 3;
            }
        }

        for (int i=0; i < vertexCount; i++) {
            int index = i * 3;
            parser::Vec3f vertex = scene.vertex_data[i];
            vertexPos[index]   = vertex.x;
            vertexPos[index + 1] = vertex.y;
            vertexPos[index + 2] = vertex.z;

            parser::Vec3f normal = scene.vnormal_data[i];
            vertexNormalPos[index]   = normal.x;
            vertexNormalPos[index + 1] = normal.y;
            vertexNormalPos[index + 2] = normal.z;
        }

	    vertexPosDataSizeInBytes = sizeof(vertexPos);
        vertexNormalPosDataSizeInBytes = sizeof(vertexNormalPos);
		indexDataSizeInBytes = sizeof(indices);

		GLuint vertexBuffer, indexBuffer;
        /*
            vertexBuffer:
                [0:vertexPosDataSizeInBytes] = vertexPos
                [vertexPosDataSizeInBytes : vertexNormalPosDataSizeInBytes] = vertexNormalPos
        */
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexPosDataSizeInBytes + vertexNormalPosDataSizeInBytes, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexPosDataSizeInBytes, vertexPos);
		glBufferSubData(GL_ARRAY_BUFFER, vertexPosDataSizeInBytes, vertexNormalPosDataSizeInBytes, vertexNormalPos);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indices, GL_STATIC_DRAW);
	}

	glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, reinterpret_cast<void*>(vertexPosDataSizeInBytes));

    size_t offset = 0;
    
    for (parser::Mesh& mesh : scene.meshes) {
        int meshIndiceCount = 0;
        for (parser::Face& face : mesh.faces) {
            meshIndiceCount += 3;
        }
        glPushMatrix();
        // transform
        for (int i = mesh.transformations.size()-1; i >= 0; i--) {
            parser::Transformation& ts = mesh.transformations[i];
            transform(ts);
        }
        // set mesh type
        if (mesh.mesh_type == "Solid") {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        // set color
        setColor(mesh.material_id);
        glDrawElements(GL_TRIANGLES, meshIndiceCount , GL_UNSIGNED_INT, reinterpret_cast<const void*>(offset));
        glPopMatrix();

        offset += sizeof(GLuint) * meshIndiceCount;
    }
}

void turnOnLights() {
    glEnable(GL_LIGHTING);
    GLfloat ambient[4] = { scene.ambient_light.x,  scene.ambient_light.y,  scene.ambient_light.z, 1.0f};
    uint count = 0;
    for (parser::PointLight& pl : scene.point_lights) {
        GLfloat pos[4] = {pl.position.x, pl.position.y, pl.position.z, 1.0f};
        GLfloat intensity[4] = {pl.intensity.x, pl.intensity.y, pl.intensity.z, 1.0f};
        glEnable(GL_LIGHT0 + count);
        glLightfv(GL_LIGHT0 + count, GL_POSITION, pos);
        glLightfv(GL_LIGHT0 + count, GL_DIFFUSE, intensity);
        glLightfv(GL_LIGHT0 + count, GL_SPECULAR, intensity);
        glLightfv(GL_LIGHT0 + count, GL_AMBIENT, ambient);
        count++;
    }
}

void fillVertexNormals() {
    size_t vertexSize = scene.vertex_data.size();
    uint* count = new uint[vertexSize];
    memset(count, 0, vertexSize);
    for (size_t i = 0; i< vertexSize; i++) {
        scene.vnormal_data.push_back(parser::Vec3f(0.0f, 0.0f, 0.0f));
    }
    for (parser::Mesh& mesh : scene.meshes) {
        for (parser::Face& face : mesh.faces) {
            // Calculate face's normal.
            size_t v0ID = face.v0_id - 1;
            size_t v1ID = face.v1_id - 1;
            size_t v2ID = face.v2_id - 1;
            count[v0ID]++;
            count[v1ID]++;
            count[v2ID]++;
            parser::Vec3f v0 = scene.vertex_data[v0ID];
            parser::Vec3f v1 = scene.vertex_data[v1ID];
            parser::Vec3f v2 = scene.vertex_data[v2ID];
            parser::Vec3f edge1 = v1 - v0;
            parser::Vec3f edge2 = v2 - v0;
            parser::Vec3f n = parser::Vec3f::cross(edge1, edge2);
            n.normalize();
            // Compute vertex normals
            scene.vnormal_data[v0ID] = (scene.vnormal_data[v0ID] * (count[v0ID] - 1) + n) / count[v0ID];
            scene.vnormal_data[v1ID] = (scene.vnormal_data[v1ID] * (count[v1ID] - 1) + n) / count[v1ID];
            scene.vnormal_data[v2ID] = (scene.vnormal_data[v2ID] * (count[v2ID] - 1) + n) / count[v2ID];
        }
    }
    delete count;
    
}


double lastTime;
int nbFrames;

void showFPS(GLFWwindow *pWindow, char* hostname)
{
    // Measure speed
     double currentTime = glfwGetTime();
     double delta = currentTime - lastTime;
	 char ss[500] = {};
     nbFrames++;
     if (delta >= 1.0 ){ // If last cout was more than 1 sec ago
         //cout << 1000.0/double(nbFrames) << endl;

         double fps = ((double)(nbFrames)) / delta;

         sprintf(ss,"CENG477 - HW3[%.2lf FPS]@%s", fps, hostname);

         glfwSetWindowTitle(pWindow, ss);

         nbFrames = 0;
         lastTime = currentTime;
     }
}

int main(int argc, char* argv[]) {

    scene.loadFromXml(argv[1]);


    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);

    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);

    fillVertexNormals();
    init();
    turnOnLights();
    initCamera();
    while(!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        drawObject();
        
        showFPS(win, hostname);

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
