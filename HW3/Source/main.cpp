#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
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
}

void setColors() {
    GLfloat ambColor[] = {1.0, 1.0, 1.0, 1.0} ;
    GLfloat diffColor[] = {0.4, 0.2, 0.0, 1.0} ;
    GLfloat specColor[] = {0.3, 0.3, 0.3, 1.0} ;
    GLfloat specExp[] = {5};
    glMaterialfv(GL_FRONT , GL_AMBIENT , ambColor);
    glMaterialfv(GL_FRONT , GL_DIFFUSE , diffColor);
    glMaterialfv(GL_FRONT , GL_SPECULAR , specColor);
    glMaterialfv(GL_FRONT , GL_SHININESS , specExp);
}

void drawObject() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static int vertexPosDataSizeInBytes;
	
    static bool firstTime = true;

	if (firstTime)
	{
		firstTime = false;
	    vertexPosDataSizeInBytes = sizeof(vertexPos);
		int indexDataSizeInBytes = sizeof(indices);
		glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

		GLuint indices[] = new GLuint(mesh.faces.size());

		GLfloat vertexPos[] = vertex_data;

        GLfloat vertexNormalPos[] = vnormal_data;

		GLuint vertexBuffer, indexBuffer, normalBuffer;
        
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexPosDataSizeInBytes, vertexPos, GL_STATIC_DRAW);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indices, GL_STATIC_DRAW);

        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_NORMAL_BUFFER, normalBuffer);
        glBufferData(GL_NORMAL_BUFFER, vertexPosDataSizeInBytes, vertexNormalPos, GL_STATIC_DRAW);
	}

	glVertexPointer(3, GL_FLOAT, 0, 0);
    glNormalPointer(GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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

    fillVertexNormals();
    init();
    initCamera();
    turnOnLights();
    while(!glfwWindowShouldClose(win)) {
        glfwWaitEvents();
        // for every object
            setColors();
            drawObject();
        //
        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
