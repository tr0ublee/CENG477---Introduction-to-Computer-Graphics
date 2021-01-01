#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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


void initCamera() {
    parser::Camera& cam = scene.camera;
    const parser::Vec3f center = cam.position + cam.gaze * cam.near_distance; 
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

    initCamera();
    while(!glfwWindowShouldClose(win)) {
        glfwWaitEvents();

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
